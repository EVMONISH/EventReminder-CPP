#include <iostream>
#include <vector>
#include<string>
#include <algorithm> // For std::sort
#include <iomanip>   // For std::put_time // Note: This header is technically not used in current version, but good for future date/time formatting

// --- Helper Structures for Date and Time ---
struct Date {
    int day;
    int month;
    int year;

    // Default constructor
    Date() : day(1), month(1), year(2000) {}

    // Parameterized constructor
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    // Comparison operator for sorting dates chronologically
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

    std::string toString() const {
        // Ensure single-digit day/month are formatted with leading zero for consistency if needed,
        // though for simple display std::to_string is fine.
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
};

struct Time {
    int hour;
    int minute;

    // Default constructor
    Time() : hour(0), minute(0) {}

    // Parameterized constructor
    Time(int h, int m) : hour(h), minute(m) {}

    // Comparison operator for sorting times
    bool operator<(const Time& other) const {
        if (hour != other.hour) return hour < other.hour;
        return minute < other.minute;
    }

    bool operator==(const Time& other) const {
        return hour == other.hour && minute == other.minute;
    }

    std::string toString() const {
        std::string h_str = std::to_string(hour);
        std::string m_str = std::to_string(minute);
        if (h_str.length() == 1) h_str = "0" + h_str;
        if (m_str.length() == 1) m_str = "0" + m_str;
        return h_str + ":" + m_str;
    }
};

// --- Event Class ---
class Event {
public:
    std::string name;
    Date date;
    Time time;
    std::string description;

    Event(std::string name, Date date, Time time, std::string description)
        : name(name), date(date), time(time), description(description) {}

    // For sorting events chronologically
    bool operator<(const Event& other) const {
        if (!(date == other.date)) return date < other.date;
        return time < other.time;
    }

    void display() const {
        std::cout << "--------------------------------------\n";
        std::cout << "Event Name:  " << name << "\n";
        std::cout << "Date:        " << date.toString() << "\n";
        std::cout << "Time:        " << time.toString() << "\n";
        std::cout << "Description: " << description << "\n";
        std::cout << "--------------------------------------\n";
    }
};

// --- EventManager Class ---
class EventManager {
private:
    std::vector<Event> events; // Our primary data structure to store events

public:
    // Add Event
    void addEvent() {
        std::string name, description;
        int d, m, y, h, min;

        std::cout << "\n--- Add New Event ---\n";
        std::cout << "Enter Event Name: ";
        std::cin.ignore(); // Clear buffer before getline
        std::getline(std::cin, name);

        std::cout << "Enter Date (DD MM YYYY): ";
        std::cin >> d >> m >> y;
        // Basic validation for date ranges (optional but good practice)
        if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1900 || y > 2100) {
            std::cout << "Invalid date. Using default (1/1/2000).\n";
            d = 1; m = 1; y = 2000;
        }
        Date date(d, m, y);

        std::cout << "Enter Time (HH MM - 24hr format): ";
        std::cin >> h >> min;
        // Basic validation for time ranges (optional but good practice)
        if (h < 0 || h > 23 || min < 0 || min > 59) {
            std::cout << "Invalid time. Using default (00:00).\n";
            h = 0; min = 0;
        }
        Time time(h, min);

        std::cout << "Enter Description: ";
        std::cin.ignore(); // Clear buffer again after reading d, m, y, h, min
        std::getline(std::cin, description);

        events.emplace_back(name, date, time, description);
        std::cout << "Event '" << name << "' added successfully!\n";
    }

    // View All Events (Sorted)
    void viewEvents() {
        if (events.empty()) {
            std::cout << "\nNo events to display.\n";
            return;
        }

        // Sort events chronologically before displaying
        std::sort(events.begin(), events.end());

        std::cout << "\n--- All Events ---\n";
        for (const auto& event : events) {
            event.display();
        }
    }

    // Search Event
    void searchEvent() {
        if (events.empty()) {
            std::cout << "\nNo events to search.\n";
            return;
        }

        std::string query;
        std::cout << "\n--- Search Event ---\n";
        std::cout << "Enter keyword (event name, description, or date DD/MM/YYYY): ";
        std::cin.ignore();
        std::getline(std::cin, query);

        bool found = false;
        // Simple linear search for demonstration
        for (const auto& event : events) {
            // Convert query to lowercase for case-insensitive search (optional enhancement)
            // std::transform(event.name.begin(), event.name.end(), event.name.begin(), ::tolower);
            // std::transform(query.begin(), query.end(), query.begin(), ::tolower);

            if (event.name.find(query) != std::string::npos ||
                event.description.find(query) != std::string::npos ||
                event.date.toString().find(query) != std::string::npos) { // Searching by date string
                event.display();
                found = true;
            }
        }
        if (!found) {
            std::cout << "No events found matching '" << query << "'.\n";
        }
    }

    // Edit Event
    void editEvent() {
        if (events.empty()) {
            std::cout << "\nNo events to edit.\n";
            return;
        }

        std::string eventName;
        std::cout << "\n--- Edit Event ---\n";
        std::cout << "Enter the NAME of the event to edit: ";
        std::cin.ignore();
        std::getline(std::cin, eventName);

        bool found = false;
        for (auto& event : events) { // Use auto& to modify the event
            if (event.name == eventName) {
                std::cout << "Event found! Enter new details (leave blank to keep current):\n";
                std::string newName, newDescription, date_input_str, time_input_str;
                int d, m, y, h, min;

                std::cout << "Current Name: " << event.name << "\n";
                std::cout << "New Event Name: ";
                std::getline(std::cin, newName);
                if (!newName.empty()) event.name = newName;

                std::cout << "Current Date: " << event.date.toString() << "\n";
                std::cout << "New Date (DD MM YYYY): ";
                std::getline(std::cin, date_input_str);
                if (!date_input_str.empty()) {
                    // Using sscanf_s for safer input on Windows, or std::istringstream
                    if (sscanf(date_input_str.c_str(), "%d %d %d", &d, &m, &y) == 3) { // Check if 3 items read
                         if (d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1900 && y <= 2100) {
                            event.date = Date(d, m, y);
                        } else {
                            std::cout << "Invalid date format or range. Date not updated.\n";
                        }
                    } else {
                         std::cout << "Invalid date format. Date not updated.\n";
                    }
                }

                std::cout << "Current Time: " << event.time.toString() << "\n";
                std::cout << "New Time (HH MM): ";
                std::getline(std::cin, time_input_str);
                if (!time_input_str.empty()) {
                    if (sscanf(time_input_str.c_str(), "%d %d", &h, &min) == 2) { // Check if 2 items read
                        if (h >= 0 && h <= 23 && min >= 0 && min <= 59) {
                            event.time = Time(h, min);
                        } else {
                            std::cout << "Invalid time format or range. Time not updated.\n";
                        }
                    } else {
                        std::cout << "Invalid time format. Time not updated.\n";
                    }
                }

                std::cout << "Current Description: " << event.description << "\n";
                std::cout << "New Description: ";
                std::getline(std::cin, newDescription);
                if (!newDescription.empty()) event.description = newDescription;

                std::cout << "Event '" << event.name << "' updated successfully!\n";
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Event with name '" << eventName << "' not found.\n";
        }
    }

    // Delete Event
    void deleteEvent() {
        if (events.empty()) {
            std::cout << "\nNo events to delete.\n";
            return;
        }

        std::string eventName;
        std::cout << "\n--- Delete Event ---\n";
        std::cout << "Enter the NAME of the event to delete: ";
        std::cin.ignore();
        std::getline(std::cin, eventName);

        // Using erase-remove idiom for efficient deletion
        auto originalSize = events.size();
        events.erase(std::remove_if(events.begin(), events.end(),
                                    [&](const Event& e) { return e.name == eventName; }),
                      events.end());

        if (events.size() < originalSize) {
            std::cout << "Event '" << eventName << "' deleted successfully!\n";
        } else {
            std::cout << "Event with name '" << eventName << "' not found.\n";
        }
    }

    // Basic Reminder Check (Optional/Advanced)
    void checkReminders() {
        if (events.empty()) {
            std::cout << "\nNo events to check for reminders.\n";
            return;
        }

        std::cout << "\n--- Upcoming Events (Sorted) ---\n";
        // Sort events to show upcoming first
        std::sort(events.begin(), events.end());

        // In a real application, you'd get the current system date and time.
        // For this console app, we'll simply list the events in chronological order
        // as "upcoming" events, assuming the user views them to see what's next.
        // Implementing true "reminders" requires platform-specific time APIs or a more
        // sophisticated date library (like <chrono> in C++11 and later)
        // and comparison logic to filter events close to the current time.

        int count = 0;
        for (const auto& event : events) {
            event.display();
            count++;
        }
        if (count == 0) {
            std::cout << "No upcoming events found (this shouldn't happen if events exist after sort).\n";
        }
    }
};

// --- Main Application ---
void displayMenu() {
    std::cout << "\n--- Event Reminder Menu ---\n";
    std::cout << "1. Add New Event\n";
    std::cout << "2. View All Events\n";
    std::cout << "3. Search Event\n";
    std::cout << "4. Edit Event\n";
    std::cout << "5. Delete Event\n";
    std::cout << "6. Check Upcoming Events (Reminder)\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    EventManager manager;
    int choice;

    do {
        displayMenu();
        // Input validation for menu choice
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        }

        switch (choice) {
            case 1:
                manager.addEvent();
                break;
            case 2:
                manager.viewEvents();
                break;
            case 3:
                manager.searchEvent();
                break;
            case 4:
                manager.editEvent();
                break;
            case 5:
                manager.deleteEvent();
                break;
            case 6:
                manager.checkReminders();
                break;
            case 7:
                std::cout << "Exiting Event Reminder. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        // Only pause if not exiting
        if (choice != 7) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear any remaining newline
            std::cin.get(); // Wait for user to press Enter
        }
    } while (choice != 7);

    return 0;
}