#include <cassert>
#include <iostream>
#include "StudyBuddyApp.cpp"  

using namespace std;

void runTests() {
    cout << "Running automated tests...\n";

    // --- Test Student creation ---
    Student s1(1, "Alice");
    assert(s1.id == 1);
    assert(s1.name == "Alice");

    // --- Test Adding Availability ---
    s1.addAvailability(TimeSlot("Mon", "10:00", "12:00"));
    assert(s1.availability.size() == 1);
    assert(s1.availability[0].day == "Mon");
    assert(s1.availability[0].startTime == "10:00");
    assert(s1.availability[0].endTime == "12:00");

    // --- Test Removing Availability ---
    s1.removeAvailability(0);
    assert(s1.availability.empty());

    // --- Test StudyBuddyApp student management ---
    StudyBuddyApp app;
    app.addStudent(2, "Bob");
    Student* bob = app.findStudentByID(2);
    assert(bob != nullptr);
    assert(bob->name == "Bob");

    // --- Test Adding Course ---
    bob->addCourse("CPSC1010");
    assert(bob->courses.size() == 1);
    assert(bob->courses[0] == "CPSC1010");

    // --- Test Overlap ---
    TimeSlot t1("Mon", "09:00", "11:00");
    TimeSlot t2("Mon", "10:00", "12:00");
    TimeSlot t3("Tue", "09:00", "11:00");
    assert(t1.overlaps(t2));   // Should overlap
    assert(!t1.overlaps(t3));  // Different day, no overlap

    cout << "✅ All tests passed!\n";
}

int main() {
    runTests();
    return 0;
}
