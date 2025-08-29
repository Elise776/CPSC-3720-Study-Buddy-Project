#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cassert>

using namespace std;

// ------------------- Helper Input Functions -------------------
string readLine() {
	string s;
	getline(cin, s);
	return s;
}

int readInt() {
	int x;
	cin >> x;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return x;
}

// ------------------- Validation Functions -------------------
bool isValidDay(const string& day) {
	vector<string> validDays = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	return find(validDays.begin(), validDays.end(), day) != validDays.end();
}

bool isValidTime(const string& time) {
	if (time.length() != 5 || time[2] != ':') return false;
	string hour = time.substr(0, 2);
	string min = time.substr(3, 2);
	return all_of(hour.begin(), hour.end(), ::isdigit) && 
		   all_of(min.begin(), min.end(), ::isdigit) &&
		   stoi(hour) >= 0 && stoi(hour) <= 23 &&
		   stoi(min) >= 0 && stoi(min) <= 59;
}

// ------------------- TimeSlot Class -------------------
class TimeSlot {
public:
	string day;
	string startTime;
	string endTime;

	TimeSlot(string d, string start, string end) : day(d), startTime(start), endTime(end) {}

	bool overlaps(const TimeSlot& other) const {
		if (day != other.day) return false;
		return !(endTime <= other.startTime || startTime >= other.endTime);
	}

	void display() const {
		cout << day << " " << startTime << "-" << endTime;
	}
};

// ------------------- Student Class -------------------
class Student {
public:
	int id;
	string name;
	vector<string> courses;
	vector<TimeSlot> availability;

	Student(int i, string n) : id(i), name(n) {}

	void addCourse(const string& course) {
		courses.push_back(course);
	}

	void addAvailability(const TimeSlot& slot) {
		availability.push_back(slot);
	}

	void removeAvailability(int index) {
		if (index >= 0 && index < availability.size())
			availability.erase(availability.begin() + index);
	}

	void viewProfile() const {
		cout << "\nStudent ID: " << id << ", Name: " << name << "\nCourses: ";
		for (const auto& c : courses) cout << c << " ";
		cout << "\nAvailability:\n";
		for (size_t i = 0; i < availability.size(); i++) {
			cout << i + 1 << ". "; 
			availability[i].display(); 
			cout << "\n";
		}
	}
};

// ------------------- StudySession Class -------------------
class StudySession {
public:
	vector<Student*> participants;
	TimeSlot time;

	StudySession(const vector<Student*>& studs, const TimeSlot& t) : participants(studs), time(t) {}

	void display() const {
		cout << "Study Session at ";
		time.display();
		cout << " with: ";
		for (auto s : participants) cout << s->name << " ";
		cout << "\n";
	}
};

// ------------------- StudyBuddyApp Class -------------------
class StudyBuddyApp {
private:
	vector<Student> students;
	vector<StudySession> sessions;

public:
	void addStudent(int id, const string& name) {
		students.push_back(Student(id, name));
		cout << "Student added successfully.\n";
	}

	Student* findStudentByID(int id) {
		for (auto& s : students) {
			if (s.id == id) return &s;
		}
		return nullptr;
	}

	vector<Student*> searchMatches(int studentID, const string& course) {
		Student* student = findStudentByID(studentID);
		vector<Student*> matches;
		if (!student) return matches;

		for (auto& other : students) {
			if (other.id == studentID) continue;
			if (find(other.courses.begin(), other.courses.end(), course) != other.courses.end()) {
				for (const auto& ts1 : student->availability) {
					for (const auto& ts2 : other.availability) {
						if (ts1.overlaps(ts2)) {
							matches.push_back(&other);
							break;
						}
					}
				}
			}
		}
		return matches;
	}

	void scheduleSession(Student* s1, Student* s2, const TimeSlot& slot) {
		bool canSchedule = false;
		for (const auto& ts1 : s1->availability) {
			for (const auto& ts2 : s2->availability) {
				if (ts1.overlaps(slot) && ts2.overlaps(slot)) {
					canSchedule = true;
					break;
				}
			}
		}
		if (canSchedule) {
			sessions.push_back(StudySession({s1, s2}, slot));
			cout << "Session scheduled successfully!\n";
		} else {
			cout << "Error: Students are not available at this time.\n";
		}
	}

	void viewAllSessions() const {
		if (sessions.empty()) {
			cout << "No scheduled sessions.\n";
			return;
		}
		cout << "\nAll Scheduled Study Sessions:\n";
		for (const auto& s : sessions) s.display();
	}
};

// ------------------- CLI Menu -------------------
void displayMenu() {
	cout << "\n--- Study Buddy Menu ---\n";
	cout << "1. Add Student\n";
	cout << "2. Add Course\n";
	cout << "3. Add Availability\n";
	cout << "4. Remove Availability\n";
	cout << "5. View Profile\n";
	cout << "6. Search Matches\n";
	cout << "7. Schedule Session\n";
	cout << "8. View All Sessions\n";
	cout << "0. Exit\n";
	cout << "Choose an option: ";
}

// ------------------- Main -------------------
int main() {
	StudyBuddyApp app;
	int choice;

	do {
		displayMenu();
		choice = readInt();

		if (choice == 1) {
			int id;
			cout << "Enter Student ID: ";
			id = readInt();
			cout << "Enter Student Name: ";
			string name = readLine();
			app.addStudent(id, name);
		}
		else if (choice == 2) {
			int id;
			cout << "Enter Student ID: ";
			id = readInt();
			Student* s = app.findStudentByID(id);
			if (!s) { cout << "Student not found.\n"; continue; }

			cout << "Enter Course: ";
			string course = readLine();
			s->addCourse(course);
			cout << "Course added successfully.\n";
		}
		else if (choice == 3) {
			int id;
			cout << "Enter Student ID: ";
			id = readInt();
			Student* s = app.findStudentByID(id);
			if (!s) { cout << "Student not found.\n"; continue; }

			string day;
			do {
				cout << "Enter Day (Mon, Tue, Wed, Thu, Fri, Sat, Sun): ";
				day = readLine();
				if (!isValidDay(day)) cout << "Invalid day. Try again.\n";
			} while (!isValidDay(day));

			string start, end;
			do {
				cout << "Enter Start Time (HH:MM): ";
				start = readLine();
				if (!isValidTime(start)) cout << "Invalid time format. Try again.\n";
			} while (!isValidTime(start));

			do {
				cout << "Enter End Time (HH:MM): ";
				end = readLine();
				if (!isValidTime(end)) cout << "Invalid time format. Try again.\n";
				else if (end <= start) cout << "End time must be after start time. Try again.\n";
			} while (!isValidTime(end) || end <= start);

			s->addAvailability(TimeSlot(day, start, end));
			cout << "Availability added successfully.\n";
		}
		else if (choice == 4) { // Remove Availability
			int id;
			cout << "Enter Student ID: ";
			id = readInt();
			Student* s = app.findStudentByID(id);
			if (!s) { cout << "Student not found.\n"; continue; }

			if (s->availability.empty()) {
				cout << "No availability to remove.\n";
				continue;
			}

			cout << "Current Availability:\n";
			for (size_t i = 0; i < s->availability.size(); i++) {
				cout << i + 1 << ". ";
				s->availability[i].display();
				cout << "\n";
			}

			int index;
			do {
				cout << "Enter the number of the availability to remove: ";
				index = readInt();
				if (index < 1 || index > s->availability.size()) cout << "Invalid number. Try again.\n";
			} while (index < 1 || index > s->availability.size());

			s->removeAvailability(index - 1);
			cout << "Availability removed successfully.\n";
		}
		else if (choice == 5) {
			int id;
			cout << "Enter Student ID: ";
			id = readInt();
			Student* s = app.findStudentByID(id);
			if (s) s->viewProfile();
			else cout << "Student not found.\n";
		}
		else if (choice == 6) {
			int id;
			cout << "Enter Your Student ID: ";
			id = readInt();
			Student* s = app.findStudentByID(id);
			if (!s) { cout << "Student not found.\n"; continue; }

			cout << "Enter Course to Search: ";
			string course = readLine();
			vector<Student*> matches = app.searchMatches(id, course);
			if (matches.empty()) cout << "No matches found.\n";
			else {
				cout << "\nMatches Found:\n";
				for (auto m : matches) cout << m->name << "\n";
			}
		}
		else if (choice == 7) {
			int id1, id2;
			cout << "Enter Student1 ID: ";
			id1 = readInt();
			cout << "Enter Student2 ID: ";
			id2 = readInt();
			Student* s1 = app.findStudentByID(id1);
			Student* s2 = app.findStudentByID(id2);
			if (!s1 || !s2) { cout << "One or both students not found.\n"; continue; }

			string day;
			do {
				cout << "Enter Day (Mon, Tue, Wed, Thu, Fri, Sat, Sun): ";
				day = readLine();
				if (!isValidDay(day)) cout << "Invalid day. Try again.\n";
			} while (!isValidDay(day));

			string start, end;
			do {
				cout << "Enter Start Time (HH:MM): ";
				start = readLine();
				if (!isValidTime(start)) cout << "Invalid time format. Try again.\n";
			} while (!isValidTime(start));

			do {
				cout << "Enter End Time (HH:MM): ";
				end = readLine();
				if (!isValidTime(end)) cout << "Invalid time format. Try again.\n";
				else if (end <= start) cout << "End time must be after start time. Try again.\n";
			} while (!isValidTime(end) || end <= start);

			app.scheduleSession(s1, s2, TimeSlot(day, start, end));
		}
		else if (choice == 8) {
			app.viewAllSessions();
		}
	} while (choice != 0);

	cout << "Exiting Study Buddy. Goodbye!\n";
	return 0;
}
