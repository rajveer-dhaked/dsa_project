# dsa_project
# 🗓️ task management with  Calendar Clone - C++ Terminal Application

> A powerful, terminal-based calendar application built in C++, inspired by Google Calendar. Offers intuitive scheduling, event management, and multiple calendar views — right from your terminal.

![Terminal Screenshot](SCREENSHOT.png)

---

## 📚 Table of Contents

- [✨ Features](#-features)
- [⚙️ Installation](#-installation)
- [🚀 Usage](#-usage)
- [🛠️ Configuration](#-configuration)
- [📁 Code Structure](#-code-structure)
- [📌 API Reference](#-api-reference)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

---

## ✨ Features

### 📝 Event Management
- ✅ Create, edit, and delete events
- ✅ Customize title, description, location
- ✅ Invite multiple attendees
- ✅ Assign priority levels: Low / Medium / High
- ✅ Support for all-day events
- ✅ Add recurring events (daily/weekly/monthly)

### 🗓️ Calendar Views
- **Day View** – Hour-by-hour schedule  
- **Week View** – 7-day grid with time slots  
- **Month View** – Overview of event density  
- **Agenda View** – Upcoming events in chronological order  
- **List View** – Full event catalog with filters  

### 🔎 Navigation & Usability
- ⏩ Jump to any date quickly  
- 🕒 One-click access to today, this week, or month  
- 🌈 Color-coded events based on priority or type  
- ⌨️ Keyboard-friendly commands for faster workflow  

---

## ⚙️ Installation

### ✅ Prerequisites
- C++11 or higher
- Compatible OS: Linux / macOS / Windows (WSL recommended for Windows)

### 🔧 Build Instructions

```bash
# 1. Clone the repository
git clone https://github.com/yourusername/calendar-clone.git
cd calendar-clone

# 2. Compile the source code
g++ -std=c++11 -o calendar main.cpp calendar.cpp event.cpp

# 3. Run the application
./calendar
