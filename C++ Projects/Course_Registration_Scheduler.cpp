// ============================================================
// Course Registration Scheduler
// Processes student enrollment requests using a priority queue,
// favouring higher academic-year students. Assigns rooms and
// generates personalised student schedules.
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <unordered_map>
// The declaration of classes (Student, course, and Room)
class Student;
class Course;
class Room;
// creating a Course public class
class Course {
public:
    std::string course_code, name, required_room_type;
    int max_capacity;
    std::vector<Student*> enrolled_students;
    std::string scheduled_time_slot;
    Room* assigned_room = nullptr;
    Course(std::string code, std::string name, std::string room_type, int capacity)
        : course_code(code), name(name), required_room_type(room_type), max_capacity(capacity) {}
    bool is_full() const {
        return enrolled_students.size() >= max_capacity;
    }
    bool enroll_student(Student* student) {
        if (!is_full()) {
            enrolled_students.push_back(student);
            return true;
        }
        return false;
    }
};
// Creating a Student public class
class Student {
public:
    std::string student_id, major;
    int academic_year;
    std::vector<Course*> enrolled_courses;

    Student(std::string id, std::string major, int year)
        : student_id(id), major(major), academic_year(year) {}

    void add_course(Course* course) {
        enrolled_courses.push_back(course);
    }
};
// Creating a Room public class
class Room {
public:
    std::string room_number, type;
    int capacity;
    std::vector<std::string> available_time_slots;
    Room(std::string number, std::string type, int capacity, std::vector<std::string> time_slots)
        : room_number(number), type(type), capacity(capacity), available_time_slots(time_slots) {}
    bool is_available(const std::string& time_slot) const {
        for (const auto& slot : available_time_slots) {
            if (slot == time_slot) return true;
        }
        return false;
    }
};
// Registration request struct and comparator for priority queue
struct RegistrationRequest {
    Student* student;
    Course* course;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    RegistrationRequest(Student* s, Course* c)
        : student(s), course(c), timestamp(std::chrono::system_clock::now()) {}
};
struct CompareRequest {
    bool operator()(const RegistrationRequest& a, const RegistrationRequest& b) {
        // Higher academic year has higher priority
        if (a.student->academic_year != b.student->academic_year)
            return a.student->academic_year < b.student->academic_year;
        return a.timestamp > b.timestamp;
    }
};
std::priority_queue<RegistrationRequest, std::vector<RegistrationRequest>, CompareRequest> request_queue;
// Scheduler class
class Scheduler {
public:
    void process_requests();
    void assign_rooms(std::vector<Course*>& courses, std::vector<Room*>& rooms);
    void resolve_conflicts();
    void generate_student_schedules(const std::vector<Student*>& students);
};
void Scheduler::process_requests() {
    while (!request_queue.empty()) {
        RegistrationRequest request = request_queue.top();
        request_queue.pop();

        Student* student = request.student;
        Course* course = request.course;

        if (course->enroll_student(student)) {
            student->add_course(course);
        }
    }
}
void Scheduler::assign_rooms(std::vector<Course*>& courses, std::vector<Room*>& rooms) {
    for (auto* course : courses) {
        for (auto* room : rooms) {
            if (room->type == course->required_room_type && room->capacity >= course->max_capacity) {
                for (const auto& slot : room->available_time_slots) {
                    if (room->is_available(slot)) {
                        course->assigned_room = room;
                        course->scheduled_time_slot = slot;
                        break;
                    }
                }
                if (course->assigned_room != nullptr) break;
            }
        }
    }
}

void Scheduler::resolve_conflicts() {
    // Implementing  conflict resolution logic (for an example having to reschedule courses to avoid room/time conflicts)
}
void Scheduler::generate_student_schedules(const std::vector<Student*>& students) {
    for (auto* student : students) {
        std::cout << "Schedule for Student " << student->student_id << ":\n";
        for (auto* course : student->enrolled_courses) {
            std::cout << "- " << course->name << " in Room " << (course->assigned_room ? course->assigned_room->room_number : "TBA")
                      << " at " << course->scheduled_time_slot << "\n";
        }
        std::cout << std::endl;
    }
}
int main() {
    // Initialize sample data
    Student* student1 = new Student("S001", "Computer Science", 3);
    Student* student2 = new Student("S002", "Mathematics", 2);
    Course* course1 = new Course("CS101", "Data Structures", "Lab", 30);
    Course* course2 = new Course("MATH101", "Calculus", "Lecture", 50);
    Room* room1 = new Room("R101", "Lab", 30, {"09:00", "10:00"});
    Room* room2 = new Room("R102", "Lecture", 50, {"10:00", "11:00"});
    // Push registration requests to priority queue
    request_queue.push(RegistrationRequest(student1, course1));
    request_queue.push(RegistrationRequest(student2, course2));
    // Create Scheduler
    Scheduler scheduler;
    // Process requests and assign rooms
    scheduler.process_requests();
    std::vector<Course*> courses = {course1, course2};
    std::vector<Room*> rooms = {room1, room2};
    scheduler.assign_rooms(courses, rooms);
    // Generate schedules for each student
    std::vector<Student*> students = {student1, student2};
    scheduler.generate_student_schedules(students);
    // Clean up the schedule
    delete student1;
    delete student2;
    delete course1;
    delete course2;
    delete room1;
    delete room2;

    return 0;
}
