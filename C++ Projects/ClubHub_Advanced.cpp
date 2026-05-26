// ============================================================
// ClubHub Advanced
// Extended club management system with file save/load support
// and a social graph (adjacency list) tracking connections
// between students who share club memberships.
// Uses insertion sort for alphabetical student listing.
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include <sstream>

class Student {
public:
    std::string firstName;
    std::string lastName;
    int id;

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
    std::set<int> memberIds;  // Store student IDs

    Club(const std::string& clubName, int clubId)
        : name(clubName), id(clubId) {}
};

class ClubHub {
private:
    std::map<int, Student> students;  // student_id -> Student
    std::map<int, Club> clubs;        // club_id -> Club
    std::map<int, std::set<int>> adjacencyList;  // student_id -> set of connected student_ids
    int nextStudentId = 1;
    int nextClubId = 1;

    void updateConnections(int clubId) {
        const std::set<int>& members = clubs[clubId].memberIds;
        for (int student1 : members) {
            for (int student2 : members) {
                if (student1 != student2) {
                    adjacencyList[student1].insert(student2);
                    adjacencyList[student2].insert(student1);
                }
            }
        }
    }

public:
    int addStudent(const std::string& firstName, const std::string& lastName) {
        int id = nextStudentId++;
        students.emplace(id, Student(firstName, lastName, id));
        return id;
    }

    int createClub(const std::string& clubName) {
        int id = nextClubId++;
        clubs.emplace(id, Club(clubName, id));
        return id;
    }

    bool addStudentToClub(int studentId, int clubId) {
        if (students.find(studentId) == students.end() || clubs.find(clubId) == clubs.end())
            return false;

        clubs[clubId].memberIds.insert(studentId);
        updateConnections(clubId);
        return true;
    }

    bool removeStudentFromClub(int studentId, int clubId) {
        if (students.find(studentId) == students.end() || clubs.find(clubId) == clubs.end())
            return false;

        clubs[clubId].memberIds.erase(studentId);

        // Clear all connections for this student
        for (int otherId : adjacencyList[studentId]) {
            adjacencyList[otherId].erase(studentId);
        }
        adjacencyList[studentId].clear();

        // Rebuild connections for clubs where the student is still a member
        for (const auto& club : clubs) {
            if (club.second.memberIds.find(studentId) != club.second.memberIds.end()) {
                updateConnections(club.first);
            }
        }
        return true;
    }

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

    std::vector<Student> findClubMembers(int clubId) {
        std::vector<Student> members;
        if (clubs.find(clubId) == clubs.end())
            return members;

        for (int studentId : clubs[clubId].memberIds) {
            members.push_back(students[studentId]);
        }
        return members;
    }

    std::vector<Student> listStudentsAlphabetically() {
        std::vector<Student> sortedStudents;
        for (const auto& pair : students) {
            sortedStudents.push_back(pair.second);
        }

        // Insertion sort in ascending order
        for (size_t i = 1; i < sortedStudents.size(); i++) {
            Student key = sortedStudents[i];
            int j = i - 1;
            while (j >= 0 && sortedStudents[j] > key) {
                sortedStudents[j + 1] = sortedStudents[j];
                j--;
            }
            sortedStudents[j + 1] = key;
        }
        return sortedStudents;
    }

    bool saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        // Save students
        file << "STUDENTS\n";
        for (const auto& pair : students) {
            const Student& student = pair.second;
            file << student.id << "," << student.firstName << ","
                 << student.lastName << "\n";
        }

        // Save clubs and their members
        file << "CLUBS\n";
        for (const auto& pair : clubs) {
            const Club& club = pair.second;
            file << club.id << "," << club.name;
            for (int memberId : club.memberIds) {
                file << "," << memberId;
            }
            file << "\n";
        }

        file.close();  // Ensure file is properly closed
        return true;
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        students.clear();
        clubs.clear();
        adjacencyList.clear();
        nextStudentId = 1;
        nextClubId = 1;

        std::string line, section;
        std::getline(file, section);  // Read "STUDENTS"

        // Read students
        while (std::getline(file, line) && line != "CLUBS") {
            std::stringstream ss(line);
            std::string id, firstName, lastName;
            std::getline(ss, id, ',');
            std::getline(ss, firstName, ',');
            std::getline(ss, lastName, ',');

            int studentId = std::stoi(id);
            students.emplace(studentId, Student(firstName, lastName, studentId));
            nextStudentId = std::max(nextStudentId, studentId + 1);
        }

        // Read clubs
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string item;
            std::vector<std::string> items;

            while (std::getline(ss, item, ',')) {
                items.push_back(item);
            }

            if (items.size() >= 2) {
                int clubId = std::stoi(items[0]);
                clubs.emplace(clubId, Club(items[1], clubId));
                nextClubId = std::max(nextClubId, clubId + 1);

                // Add members
                for (size_t i = 2; i < items.size(); i++) {
                    int memberId = std::stoi(items[i]);
                    clubs[clubId].memberIds.insert(memberId);
                }
                updateConnections(clubId);
            }
        }

        file.close();  // Ensure file is properly closed
        return true;
    }
};
