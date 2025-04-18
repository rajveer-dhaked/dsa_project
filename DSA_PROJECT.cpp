#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <cctype>

using namespace std;

// ==================== Utility Functions ====================
string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return tolower(c); });
    return result;
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

int safeStoi(const string& input, int fallback = 0) {
    try {
        return stoi(input);
    } catch (...) {
        return fallback;
    }
}

string getInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    return trim(input);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void waitForEnter(const string& message = "\nPress Enter to continue...") {
    cout << message;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

time_t stringToTime(const string& date_str, const string& format = "%Y-%m-%d %H:%M") {
    tm timeinfo = {};
    istringstream ss(date_str);
    ss >> get_time(&timeinfo, format.c_str());
    if (ss.fail()) {
        return 0;
    }
    return mktime(&timeinfo);
}

string timeToString(time_t t, const string& format = "%Y-%m-%d %H:%M") {
    char buffer[20];
    strftime(buffer, sizeof(buffer), format.c_str(), localtime(&t));
    return buffer;
}

string dateToString(time_t t) {
    return timeToString(t, "%Y-%m-%d");
}

string getTimePart(time_t t) {
    return timeToString(t, "%H:%M");
}

string getDayName(time_t t) {
    return timeToString(t, "%A");
}

// ==================== Enums ====================
enum class Priority { LOW, MEDIUM, HIGH };
enum class Color { RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, GRAY, DEFAULT };

string toString(Priority priority) {
    switch (priority) {
        case Priority::LOW: return "Low";
        case Priority::MEDIUM: return "Medium";
        case Priority::HIGH: return "High";
    }
    return "Unknown";
}

string toString(Color color) {
    switch (color) {
        case Color::RED: return "Red";
        case Color::BLUE: return "Blue";
        case Color::GREEN: return "Green";
        case Color::YELLOW: return "Yellow";
        case Color::PURPLE: return "Purple";
        case Color::ORANGE: return "Orange";
        case Color::GRAY: return "Gray";
        case Color::DEFAULT: return "Default";
    }
    return "Unknown";
}

// ANSI color codes for terminal output
    // Update the TermColor namespace to include the missing codes
namespace TermColor {
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string BLUE = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN = "\033[36m";
    const string WHITE = "\033[37m";
    const string RESET = "\033[0m";
    const string BOLD = "\033[1m";
    const string UNDERLINE = "\033[4m";
    const string BG_BLUE = "\033[44m";
}

string getColorCode(Color color) {
    switch (color) {
        case Color::RED: return TermColor::RED;
        case Color::GREEN: return TermColor::GREEN;
        case Color::YELLOW: return TermColor::YELLOW;
        case Color::BLUE: return TermColor::BLUE;
        case Color::PURPLE: return TermColor::MAGENTA;
        case Color::ORANGE: return TermColor::YELLOW;
        case Color::GRAY: return TermColor::WHITE;
        default: return TermColor::CYAN;
    }
}

// ==================== Event Class ====================
class Event {
private:
    static int next_id;

public:
    int id;
    string title;
    time_t start_time;
    time_t end_time;
    Color color;
    Priority priority;
    string description;
    string location;
    vector<string> attendees;
    bool is_all_day;
    bool is_recurring;
    string recurrence_pattern;

    Event(const string& title, time_t start, time_t end, 
          Color color = Color::DEFAULT, Priority priority = Priority::MEDIUM,
          const string& desc = "", const string& loc = "", 
          const vector<string>& att = {}, bool all_day = false,
          bool recurring = false, const string& recur_pattern = "")
        : id(next_id++), title(title), start_time(start), end_time(end),
          color(color), priority(priority), description(desc),
          location(loc), attendees(att), is_all_day(all_day),
          is_recurring(recurring), recurrence_pattern(recur_pattern) {}

    bool isSameDay(time_t day) const {
        tm t1 = *localtime(&start_time);
        tm t2 = *localtime(&day);
        return (t1.tm_year == t2.tm_year && t1.tm_mon == t2.tm_mon && t1.tm_mday == t2.tm_mday);
    }

    bool isBetween(time_t start, time_t end) const {
        return (start_time >= start && start_time <= end) || 
               (end_time >= start && end_time <= end) ||
               (start_time <= start && end_time >= end);
    }

    bool isAtTime(time_t time) const {
        return start_time <= time && end_time >= time;
    }

    void printSummary(bool detailed = false) const {
        string color_code = getColorCode(color);
        cout << color_code << "[" << id << "] " << title << TermColor::RESET << " (";
        if (is_all_day) {
            cout << dateToString(start_time) << " - All Day";
        } else {
            cout << timeToString(start_time) << " - " << timeToString(end_time);
        }
        cout << ") [" << toString(priority) << "]";
        
        if (detailed) {
            cout << "\n  " << color_code << "Color: " << toString(color) << TermColor::RESET;
            if (!location.empty()) cout << "\n  Location: " << location;
            if (!description.empty()) cout << "\n  Description: " << description;
            if (!attendees.empty()) {
                cout << "\n  Attendees: ";
                for (const auto& name : attendees) cout << name << ", ";
            }
            if (is_recurring) cout << "\n  Recurring: " << recurrence_pattern;
        }
        cout << endl;
    }

    void printDetails() const {
        string color_code = getColorCode(color);
        cout << TermColor::BOLD << "=== Event Details ===" << TermColor::RESET << endl;
        cout << color_code << "Title: " << title << TermColor::RESET << endl;
        cout << "Time: ";
        if (is_all_day) {
            cout << dateToString(start_time) << " (All Day)";
        } else {
            cout << timeToString(start_time) << " to " << timeToString(end_time);
        }
        cout << "\nPriority: " << toString(priority);
        cout << "\nColor: " << color_code << toString(color) << TermColor::RESET;
        if (!location.empty()) cout << "\nLocation: " << location;
        if (!description.empty()) cout << "\nDescription: " << description;
        if (!attendees.empty()) {
            cout << "\nAttendees: ";
            for (const auto& name : attendees) cout << name << ", ";
        }
        if (is_recurring) cout << "\nRecurrence: " << recurrence_pattern;
        cout << endl;
    }
};

int Event::next_id = 1;

// ==================== Calendar Class ====================
class Calendar {
private:
    vector<Event> events;
    string name;
    string owner;

    void sortEvents() {
        sort(events.begin(), events.end(), 
            [](const Event& a, const Event& b) { return a.start_time < b.start_time; });
    }

public:
    Calendar(const string& name = "My Calendar", const string& owner = "User")
        : name(name), owner(owner) {}

    void addEvent(const Event& event) {
        events.push_back(event);
        sortEvents();
    }

    bool deleteEvent(int id) {
        auto it = remove_if(events.begin(), events.end(), 
            [id](const Event& e) { return e.id == id; });
        if (it != events.end()) {
            events.erase(it, events.end());
            return true;
        }
        return false;
    }

    Event* findEvent(int id) {
        for (auto& e : events) {
            if (e.id == id) return &e;
        }
        return nullptr;
    }

    vector<Event> getEventsForDay(time_t day) const {
        vector<Event> result;
        for (const auto& e : events) {
            if (e.isSameDay(day)) {
                result.push_back(e);
            }
        }
        return result;
    }

    vector<Event> getEventsBetween(time_t start, time_t end) const {
        vector<Event> result;
        for (const auto& e : events) {
            if (e.isBetween(start, end)) {
                result.push_back(e);
            }
        }
        return result;
    }

// Fix the displayDay function - remove the UNDERLINE usage or replace with BOLD
void displayDay(time_t day) const {
    clearScreen();
    auto day_events = getEventsForDay(day);
    
    cout << TermColor::BOLD << "\n=== " << getDayName(day) << " " << dateToString(day) 
         << " ===" << TermColor::RESET << "\n\n";
    
    if (day_events.empty()) {
        cout << "No events scheduled for this day.\n";
    } else {
        for (const auto& e : day_events) {
            e.printSummary(true);
            cout << string(60, '-') << "\n";
        }
    }
    waitForEnter();
}

// Fix the displayWeek function - remove BG_BLUE or replace with BLUE
void displayWeek(time_t reference_day) const {
    clearScreen();
    tm ref = *localtime(&reference_day);
    ref.tm_mday -= ref.tm_wday; // Start from Sunday
    mktime(&ref);

    vector<tm> week_days(7);
    for (int i = 0; i < 7; ++i) {
        week_days[i] = ref;
        week_days[i].tm_mday += i;
        mktime(&week_days[i]);
    }

    cout << TermColor::BOLD << "\n=== Week View (" 
         << dateToString(mktime(&week_days.front()))
         << " to "
         << dateToString(mktime(&week_days.back()))
         << ") ===" << TermColor::RESET << "\n\n";

    // Print day headers
    cout << setw(10) << "Time";
    for (const auto& day : week_days) {
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%a %m/%d", &day);
        cout << setw(20) << buffer;
    }
    cout << '\n' << string(150, '-') << '\n';

    // Print hourly grid
    for (int hour = 8; hour <= 20; ++hour) {
        cout << setw(10) << (hour <= 12 ? to_string(hour) + " AM" : 
                           (hour == 12 ? "12 PM" : to_string(hour - 12) + " PM"));
        
        for (const auto& day : week_days) {
            time_t current_time = mktime((tm*)&day) + hour * 3600;
            bool event_printed = false;
            
            for (const auto& e : events) {
                if (e.isAtTime(current_time)) {
                    string title = e.title.substr(0, 18);
                    cout << getColorCode(e.color) << setw(20) << title << TermColor::RESET;
                    event_printed = true;
                    break;
                }
            }
            if (!event_printed) cout << setw(20) << "";
        }
        cout << '\n';
    }

    // Print all-day events
    cout << "\n" << TermColor::BOLD << "All-Day Events:" << TermColor::RESET << "\n";
    for (const auto& day : week_days) {
        time_t day_time = mktime((tm*)&day);
        auto day_events = getEventsForDay(day_time);
        
        bool has_all_day = false;
        for (const auto& e : day_events) {
            if (e.is_all_day) {
                if (!has_all_day) {
                    cout << setw(10) << dateToString(day_time) << ": ";
                    has_all_day = true;
                }
                cout << getColorCode(e.color) << "[" << e.title << "] " << TermColor::RESET;
            }
        }
        if (has_all_day) cout << '\n';
    }
    waitForEnter();
}
    void displayMonth(time_t current_date) const {
        clearScreen();
        tm t = *localtime(&current_date);
        t.tm_mday = 1;
        mktime(&t);
        
        int first_day = t.tm_wday;
        int days_in_month = 31;
        
        t.tm_mon++;
        t.tm_mday = 0;
        mktime(&t);
        days_in_month = t.tm_mday;

        cout << TermColor::BOLD << "\n=== Calendar for " << put_time(&t, "%B %Y") 
             << " ===" << TermColor::RESET << "\n\n";
        cout << " Sun Mon Tue Wed Thu Fri Sat\n";

        // Get events for this month
        time_t month_start = mktime(&t);
        t.tm_mday = days_in_month;
        time_t month_end = mktime(&t);
        auto month_events = getEventsBetween(month_start, month_end);

        for (int i = 0; i < first_day; ++i) cout << "    ";
        for (int day = 1; day <= days_in_month; ++day) {
            t.tm_mday = day;
            mktime(&t);
            bool has_event = false;
            
            for (const auto& e : month_events) {
                if (e.isSameDay(mktime(&t))) {
                    has_event = true;
                    break;
                }
            }
            
            if (has_event) {
                cout << TermColor::BOLD << "[" << setw(2) << day << "]" << TermColor::RESET;
            } else {
                cout << " " << setw(2) << day << " ";
            }
            
            if ((day + first_day) % 7 == 0) cout << '\n';
        }
        cout << "\n\n" << TermColor::BOLD << "Legend: " << TermColor::RESET 
             << "[##] = Day with events\n";
        waitForEnter();
    }

    void displayAgenda(time_t start, time_t end) const {
        clearScreen();
        auto agenda_events = getEventsBetween(start, end);
        
        cout << TermColor::BOLD << "\n=== Agenda View (" 
             << dateToString(start) << " to " << dateToString(end) 
             << ") ===" << TermColor::RESET << "\n\n";
        
        if (agenda_events.empty()) {
            cout << "No events found in this period.\n";
        } else {
            string current_date;
            for (const auto& e : agenda_events) {
                string event_date = dateToString(e.start_time);
                if (event_date != current_date) {
                    current_date = event_date;
                    cout << "\n" << TermColor::BOLD << current_date << " (" 
                         << getDayName(e.start_time) << ")" << TermColor::RESET << "\n";
                    cout << string(60, '=') << "\n";
                }
                e.printSummary(true);
                cout << string(60, '-') << "\n";
            }
        }
        waitForEnter();
    }

    void listAllEvents() const {
        clearScreen();
        cout << TermColor::BOLD << "\n=== All Events ===" << TermColor::RESET << "\n\n";
        
        if (events.empty()) {
            cout << "No events in calendar.\n";
        } else {
            for (const auto& e : events) {
                e.printSummary(true);
                cout << string(60, '=') << "\n";
            }
        }
        waitForEnter();
    }
};

// ==================== Calendar UI Class ====================
class CalendarUI {
private:
    Calendar calendar;
    time_t current_date;

    time_t promptDate(const string& prompt, bool include_time = true) {
        while (true) {
            string format = include_time ? "YYYY-MM-DD HH:MM" : "YYYY-MM-DD";
            string input = getInput(prompt + " (" + format + "): ");
            time_t t = stringToTime(input, include_time ? "%Y-%m-%d %H:%M" : "%Y-%m-%d");
            if (t != 0) return t;
            cout << "Invalid format. Please try again.\n";
        }
    }

    Color promptColor() {
        map<string, Color> color_map = {
            {"red", Color::RED}, {"blue", Color::BLUE}, {"green", Color::GREEN},
            {"yellow", Color::YELLOW}, {"purple", Color::PURPLE}, {"orange", Color::ORANGE},
            {"gray", Color::GRAY}, {"", Color::DEFAULT}
        };

        while (true) {
            cout << "Available colors:\n";
            for (const auto& pair : color_map) {
                if (!pair.first.empty()) {
                    cout << getColorCode(pair.second) << pair.first << TermColor::RESET << " ";
                }
            }
            cout << "\nDefault color will be used if empty\n";

            string input = toLower(getInput("Color: "));
            if (color_map.count(input)) {
                return color_map[input];
            } else if (input.empty()) {
                return Color::DEFAULT;
            }
            cout << "Invalid color. Please try again.\n";
        }
    }

    Priority promptPriority() {
        while (true) {
            string input = toLower(getInput("Priority (Low/Medium/High) [Medium]: "));
            if (input == "high") return Priority::HIGH;
            if (input == "low") return Priority::LOW;
            if (input.empty() || input == "medium") return Priority::MEDIUM;
            cout << "Invalid priority. Please try again.\n";
        }
    }

    vector<string> promptAttendees() {
        vector<string> attendees;
        cout << "Enter attendees (one per line, empty to finish):\n";
        while (true) {
            string name = getInput("> ");
            if (name.empty()) break;
            attendees.push_back(name);
        }
        return attendees;
    }
    std::string timeToString(time_t time) {
        tm* local_tm = localtime(&time);  // Convert to local time
        
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_tm);
        return std::string(buffer);
    }


    void addEvent() {
    clearScreen();
    cout << TermColor::BOLD << "=== Add New Event ===" << TermColor::RESET << "\n\n";

    string title = getInput("Event Title: ");
    while (title.empty()) {
        cout << "Title cannot be empty!\n";
        title = getInput("Event Title: ");
    }

    // Get current date and time
    time_t now = time(nullptr);
    tm* local_tm = localtime(&now);
    
    // Set default start time to current time
    time_t start = mktime(local_tm);
    
    // Get priority first as it affects time handling
    Priority priority = promptPriority();
    
    time_t end;
    string start_date = dateToString(start);
    string start_time = getTimePart(start);
    
    if (priority == Priority::LOW) {
        // For LOW priority - use current date, ask for start time, duration fixed to 1 hour
        string time_input = getInput("Start time (HH:MM) [" + start_time + "]: ");
        if (!time_input.empty()) {
            string full_datetime = start_date + " " + time_input;
            time_t new_start = stringToTime(full_datetime, "%Y-%m-%d %H:%M");
            if (new_start != 0) {
                start = new_start;
            } else {
                cout << "Invalid time format, using current time.\n";
            }
        }
        end = start + 3600; // Fixed 1 hour duration
        cout << "Low priority - duration set to 1 hour\n";
    } 
    else if (priority == Priority::MEDIUM) {
        // For MEDIUM priority - use current date, ask for start time, duration fixed to 2 hours
        string time_input = getInput("Start time (HH:MM) [" + start_time + "]: ");
        if (!time_input.empty()) {
            string full_datetime = start_date + " " + time_input;
            time_t new_start = stringToTime(full_datetime, "%Y-%m-%d %H:%M");
            if (new_start != 0) {
                start = new_start;
            } else {
                cout << "Invalid time format, using current time.\n";
            }
        }
        end = start + 7200; // Fixed 2 hours duration
        cout << "Medium priority - duration set to 2 hours\n";
    }
    else { // HIGH priority
        // For HIGH priority - ask for full start and end datetime
        string default_start = timeToString(start);
        string start_input = getInput("Start date & time (YYYY-MM-DD HH:MM) [" + default_start + "]: ");
        if (!start_input.empty()) {
            time_t new_start = stringToTime(start_input, "%Y-%m-%d %H:%M");
            if (new_start != 0) {
                start = new_start;
            } else {
                cout << "Invalid format, using current time.\n";
            }
        }
        
        string default_end = timeToString(start + 3600);
        string end_input = getInput("End date & time (YYYY-MM-DD HH:MM) [" + default_end + "]: ");
        if (!end_input.empty()) {
            end = stringToTime(end_input, "%Y-%m-%d %H:%M");
            while (end == 0 || difftime(end, start) <= 0) {
                if (end == 0) {
                    cout << "Invalid format. ";
                } else {
                    cout << "End time must be after start time! ";
                }
                end_input = getInput("Please enter end time (YYYY-MM-DD HH:MM): ");
                end = stringToTime(end_input, "%Y-%m-%d %H:%M");
            }
        } else {
            end = start + 3600; // Default 1 hour if not specified
        }
    }

    string desc = getInput("Description (optional): ");
    string loc = getInput("Location (optional): ");
    
    // Set color based on priority
    Color color;
    switch (priority) {
        case Priority::LOW: color = Color::BLUE; break;
        case Priority::MEDIUM: color = Color::GREEN; break;
        case Priority::HIGH: color = Color::RED; break;
    }
    cout << "Auto-selected color: " << getColorCode(color) 
         << toString(color) << TermColor::RESET << "\n";

    vector<string> attendees = promptAttendees();

    Event new_event(title, start, end, color, priority, desc,loc, attendees);
    calendar.addEvent(new_event);

    cout << TermColor::GREEN << "\nEvent added successfully with ID: " 
         << new_event.id << "!" << TermColor::RESET << "\n";
    waitForEnter();
}
void editEvent() {
    clearScreen();
    cout << TermColor::BOLD << "=== Edit Event ===" << TermColor::RESET << "\n\n";
    
    int id = safeStoi(getInput("Enter event ID to edit: "), -1);
    Event* event = calendar.findEvent(id);
    if (!event) {
        cout << TermColor::RED << "Event not found!" << TermColor::RESET << "\n";
        waitForEnter();
        return;
    }
    
    // Show current details
    cout << "Current details:\n";
    event->printDetails();
    cout << "\nLeave blank to keep current value.\n";
    
    // Edit title
    string new_title = getInput("New title [" + event->title + "]: ");
    if (!new_title.empty()) event->title = new_title;
    
    // Edit start time
    string current_start = event->is_all_day ? dateToString(event->start_time) : timeToString(event->start_time);
    string start_input = getInput("New start time [" + current_start + "]: ");
    if (!start_input.empty()) {
        time_t new_start = stringToTime(start_input, event->is_all_day ? "%Y-%m-%d" : "%Y-%m-%d %H:%M");
        if (new_start != 0) {
            time_t duration = event->end_time - event->start_time;
            event->start_time = new_start;
            event->end_time = new_start + duration;
        } else {
            cout << "Invalid time format, keeping original.\n";
        }
    }
    
    // Edit end time (if not all-day)
    if (!event->is_all_day) {
        string end_input = getInput("New end time [" + timeToString(event->end_time) + "]: ");
        if (!end_input.empty()) {
            time_t new_end = stringToTime(end_input, "%Y-%m-%d %H:%M");
            if (new_end != 0 && new_end > event->start_time) {
                event->end_time = new_end;
            } else {
                cout << "Invalid time or before start time, keeping original.\n";
            }
        }
    }
    
    // Edit priority and color
    cout << "Current priority: " << toString(event->priority) << "\n";
    string priority_input = getInput("Change priority? (y/n) [n]: ");
    if (toLower(priority_input) == "y") {
        Priority old_priority = event->priority;
        event->priority = promptPriority();
        
        // Update color if priority changed
        if (event->priority != old_priority) {
            switch (event->priority) {
                case Priority::LOW: event->color = Color::BLUE; break;
                case Priority::MEDIUM: event->color = Color::GREEN; break;
                case Priority::HIGH: event->color = Color::RED; break;
            }
            cout << "Color updated to: " << getColorCode(event->color) 
                 << toString(event->color) << TermColor::RESET << "\n";
        }
    }
    
    // Edit description
    string new_desc = getInput("New description [" + event->description + "]: ");
    if (!new_desc.empty()) event->description = new_desc;
    
    // Edit location
    string new_loc = getInput("New location [" + event->location + "]: ");
    if (!new_loc.empty()) event->location = new_loc;
    
    // Edit attendees
    cout << "Current attendees: ";
    if (event->attendees.empty()) {
        cout << "None\n";
    } else {
        for (const auto& name : event->attendees) cout << name << ", ";
        cout << "\n";
    }
    string att_input = getInput("Edit attendees? (y/n) [n]: ");
    if (toLower(att_input) == "y") {
        event->attendees = promptAttendees();
    }
    
    // Edit all-day status
    string all_day_input = getInput("All-day event? (current: " + string(event->is_all_day ? "Yes" : "No") + ") (y/n) [" + (event->is_all_day ? "y" : "n") + "]: ");
    if (!all_day_input.empty()) {
        event->is_all_day = (toLower(all_day_input) == "y");
    }
    
    // Edit recurrence
    if (event->is_recurring) {
        string recur_input = getInput("Current recurrence: " + event->recurrence_pattern + "\nChange recurrence pattern? (y/n) [n]: ");
        if (toLower(recur_input) == "y") {
            event->recurrence_pattern = getInput("New recurrence pattern (Daily/Weekly/Monthly): ");
        }
    } else {
        string recur_input = getInput("Make this a recurring event? (y/n) [n]: ");
        if (toLower(recur_input) == "y") {
            event->is_recurring = true;
            event->recurrence_pattern = getInput("Recurrence pattern (Daily/Weekly/Monthly): ");
        }
    }
    
    cout << TermColor::GREEN << "\nEvent updated successfully!" << TermColor::RESET << "\n";
    waitForEnter();
}
    

    void deleteEvent() {
        clearScreen();
        cout << TermColor::BOLD << "=== Delete Event ===" << TermColor::RESET << "\n\n";

        int id = safeStoi(getInput("Enter event ID to delete: "), -1);
        if (calendar.deleteEvent(id)) {
            cout << TermColor::GREEN << "Event deleted successfully!" << TermColor::RESET << "\n";
        } else {
            cout << TermColor::RED << "Event not found!" << TermColor::RESET << "\n";
        }
        waitForEnter();
    }

    
void viewEventDetails() {
    clearScreen();
    cout << TermColor::BOLD << "=== Event Details ===" << TermColor::RESET << "\n\n";

    int id = safeStoi(getInput("Enter event ID to view: "), -1);
    Event* event = calendar.findEvent(id);
    if (event) {
        string color_code = getColorCode(event->color);
        cout << TermColor::BOLD << "=== Event Details ===" << TermColor::RESET << endl;
        cout << color_code << "Title: " << event->title << TermColor::RESET << endl;
        cout << "Time: ";
        if (event->is_all_day) {
            cout << dateToString(event->start_time) << " (All Day)";
        } else {
            cout << timeToString(event->start_time) << " to " << timeToString(event->end_time);
        }
        cout << "\nPriority: " << toString(event->priority);
        cout << "\nColor: " << color_code << toString(event->color) << TermColor::RESET;
        if (!event->location.empty()) cout << "\nLocation: " << event->location;
        if (!event->description.empty()) cout << "\nDescription: " << event->description;
        if (!event->attendees.empty()) {
            cout << "\nAttendees: ";
            for (const auto& name : event->attendees) cout << name << ", ";
        }
        cout << "\nAll-day event: " << (event->is_all_day ? "Yes" : "No");
        if (event->is_recurring) cout << "\nRecurrence: " << event->recurrence_pattern;
        cout << endl;
    } else {
        cout << TermColor::RED << "Event not found!" << TermColor::RESET << "\n";
    }
    waitForEnter();
}

    void navigateToDate() {
        clearScreen();
        cout << TermColor::BOLD << "=== Navigate to Date ===" << TermColor::RESET << "\n\n";
        current_date = promptDate("Enter date", false);
        cout << "\nNow viewing: " << TermColor::BOLD << dateToString(current_date) 
             << TermColor::RESET << "\n";
        waitForEnter();
    }

    void showMainMenu() {
        clearScreen();
        cout << TermColor::BOLD << "=== Google Calendar Clone ===" << TermColor::RESET << "\n";
        cout << "Today is " << TermColor::BOLD << dateToString(time(nullptr)) 
             << TermColor::RESET << "\n\n";
        
        cout << "[D]ay View    [W]eek View    [M]onth View\n";
        cout << "[A]genda View [L]ist All Events\n";
        cout << "[N]ew Event   [E]dit Event   [X] Delete Event\n";
        cout << "[V]iew Event  [G]o to Date   [Q]uit\n\n";
    }

public:
    CalendarUI() : current_date(time(nullptr)) {}

    void run() {
        char choice;
        do {
            showMainMenu();
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore();
            choice = tolower(choice);

            switch (choice) {
                case 'd': calendar.displayDay(current_date); break;
                case 'w': calendar.displayWeek(current_date); break;
                case 'm': calendar.displayMonth(current_date); break;
                case 'a': {
                    time_t start = promptDate("Start of Agenda View");
                    time_t end = promptDate("End of Agenda View");
                    calendar.displayAgenda(start, end);
                    break;
                }
                case 'l': calendar.listAllEvents(); break;
                case 'n': addEvent(); break;
                case 'e': editEvent(); break;
                case 'x': deleteEvent(); break;
                case 'v': viewEventDetails(); break;
                case 'g': navigateToDate(); break;
                case 'q': cout << "Exiting...\n"; break;
                default: 
                    cout << TermColor::RED << "Invalid choice!" << TermColor::RESET << "\n";
                    waitForEnter("Press Enter to try again...");
                    break;
            }
        } while (choice != 'q');
    }
};

// ==================== Main Function ====================
int main() {
    CalendarUI ui;
    ui.run();
    return 0;
}