// ============================================================
// ClubHub Basic
// Basic student club management system using STL containers.
// Supports adding students, creating clubs, managing membership,
// and listing students alphabetically. No file persistence.
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
class Student {
public:
    std::string firstName;
    std::string lastName;
    int id;

    // Default constructor
    Student() : firstName(""), lastName(""), id(0) {}

    Student(const std::string& first, const std::string& last, int studentId)
        : firstName(first), lastName(last), id(studentId) {}

    bool operator<(const Student& other) const {
        if (lastName == other.lastName)
            return firstName < other.firstName;
        return lastName < other.lastName;
    }
};
class Club {
public:
    std::string name;
    int id;
    std::set<int> memberIds;

    // Default constructor
    Club() : name(""), id(0) {}

    Club(const std::string& clubName, int clubId)
        : name(clubName), id(clubId) {}
};
class ClubHub {
private:
    std::map<int, Student> students;
    std::map<int, Club> clubs;
    int nextStudentId = 1;
    int nextClubId = 1;
public:
    // Add a new student to the system
    int addStudent(const std::string& firstName, const std::string& lastName) {
        int id = nextStudentId++;
        students.emplace(id, Student(firstName, lastName, id));
        return id;
    }
    // Creating a new club
    int createClub(const std::string& clubName) {
        int id = nextClubId++;
        clubs.emplace(id, Club(clubName, id));
        return id;
    }

    // Add a student to a club
    bool addStudentToClub(int studentId, int clubId) {
        if (students.find(studentId) == students.end() || clubs.find(clubId) == clubs.end())
            return false;

        clubs[clubId].memberIds.insert(studentId);
        return true;
    }
    // Remove a student from a club
    bool removeStudentFromClub(int studentId, int clubId) {
        if (students.find(studentId) == students.end() || clubs.find(clubId) == clubs.end())
            return false;

        clubs[clubId].memberIds.erase(studentId);
        return true;
    }
    // Find all clubs a student is a member of
    std::vector<std::string> findStudentClubs(int studentId) {
        std::vector<std::string> studentClubs;
        if (students.find(studentId) == students.end())
            return studentClubs;

        for (const auto& club : clubs) {
            if (club.second.memberIds.find(studentId) != club.second.memberIds.end()) {
                studentClubs.push_back(club.second.name);
            }
        }
        return studentClubs;
    }
    // Find all students in a specific club
    std::vector<Student> findClubMembers(int clubId) {
        std::vector<Student> members;
        if (clubs.find(clubId) == clubs.end())
            return members;

        for (int studentId : clubs[clubId].memberIds) {
            members.push_back(students[studentId]);
        }
        return members;
    }
    // List all students alphabetically by last name
    std::vector<Student> listStudentsAlphabetically() {
        std::vector<Student> sortedStudents;
        for (const auto& pair : students) {
            sortedStudents.push_back(pair.second);
        }

        std::sort(sortedStudents.begin(), sortedStudents.end());
        return sortedStudents;
    }

    // Getter for student's name based on ID
    std::string getStudentName(int studentId) const {
        if (students.find(studentId) != students.end()) {
            return students.at(studentId).firstName + " " + students.at(studentId).lastName;
        }
        return "";
    }
};
int main() {
    ClubHub hub;
    // Add some students and clubs
    int student1 = hub.addStudent("Mbuso", "Zulu");
    int student2 = hub.addStudent("Funanai", "Rambanduli");
    int club1 = hub.createClub("Football");
    int club2 = hub.createClub("Swimming");
    // Add students to clubs
    hub.addStudentToClub(student1, club1);
    hub.addStudentToClub(student2, club1);
    hub.addStudentToClub(student2, club2);
    // List all students alphabetically
    std::vector<Student> sortedStudents = hub.listStudentsAlphabetically();
    for (const auto& student : sortedStudents) {
        std::cout << student.firstName << " " << student.lastName << std::endl;
    }
    // Find all clubs a student is a member of
    std::vector<std::string> studentClubs = hub.findStudentClubs(student2);
    std::cout << "Clubs " << hub.getStudentName(student2) << " is a member of:" << std::endl;
    for (const auto& club : studentClubs) {
        std::cout << "- " << club << std::endl;
    }

    // Find all members of a club
    std::vector<Student> clubMembers = hub.findClubMembers(club1);
    std::cout << "Members of the Football Club:" << std::endl;
    for (const auto& member : clubMembers) {
        std::cout << "- " << member.firstName << " " << member.lastName << std::endl;
    }
    return 0;
}
