#include <iostream>
#include <string>

struct Ticket {
    int ticketID;
    std::string customerName;
    std::string issueDescription;
    std::string status;
    Ticket* next;

    Ticket(int id, std::string name, std::string description, std::string stat)
        : ticketID(id), customerName(name), issueDescription(description), status(stat), next(nullptr) {}
};

//Creating a Private Class for Ticket List
class TicketList {
private:
    Ticket* head;

// Creating a Public class for ticket list
public:
    TicketList() : head(nullptr) {}

    void addTicket(int id, std::string name, std::string description, std::string stat) {
        Ticket* newTicket = new Ticket(id, name, description, stat);
        if (head == nullptr) {
            head = newTicket;
        } else {
            Ticket* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newTicket;
        }
    }

    void removeTicket(int id) {
        if (head == nullptr) return;

        if (head->ticketID == id) {
            Ticket* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Ticket* temp = head;
        while (temp->next != nullptr && temp->next->ticketID != id) {
            temp = temp->next;
        }

        if (temp->next == nullptr) return;

        Ticket* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
    }

    Ticket* findTicket(int id) {
        Ticket* temp = head;
        while (temp != nullptr) {
            if (temp->ticketID == id) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void printAllTickets() {
        Ticket* temp = head;
        while (temp != nullptr) {
            std::cout << "Ticket ID: " << temp->ticketID
                      << ", Customer Name: " << temp->customerName
                      << ", Issue: " << temp->issueDescription
                      << ", Status: " << temp->status << std::endl;
            temp = temp->next;
        }
    }

    void insertAfter(int targetID, int id, std::string name, std::string description, std::string stat) {
        Ticket* target = findTicket(targetID);
        if (target != nullptr) {
            Ticket* newTicket = new Ticket(id, name, description, stat);
            newTicket->next = target->next;
            target->next = newTicket;
        }
    }

    void deleteAfter(int targetID) {
        Ticket* target = findTicket(targetID);
        if (target != nullptr && target->next != nullptr) {
            Ticket* toDelete = target->next;
            target->next = toDelete->next;
            delete toDelete;
        }
    }

    int findLength() {
        int length = 0;
        Ticket* temp = head;
        while (temp != nullptr) {
            length++;
            temp = temp->next;
        }
        return length;
    }

    Ticket* returnFirst() {
        return head;
    }

    Ticket* returnLast() {
        Ticket* temp = head;
        if (temp == nullptr) return nullptr;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        return temp;
    }

    bool isEmpty() {
        return head == nullptr;
    }
};

int main() {
    TicketList ticketList;

    // Add 5 Tickets
    ticketList.addTicket(1, "Mbuso", "Login issue", "Open");
    ticketList.addTicket(2, "Funanani", "Software crash", "Open");
    ticketList.addTicket(3, "Ndabezitha", "Payment failure", "Closed");
    ticketList.addTicket(4, "Osikiwa", "Account blocked", "Open");
    ticketList.addTicket(5, "Mageba", "Network issue", "In Progress");

    // Print Initial List
    std::cout << "Initial List:" << std::endl;
    ticketList.printAllTickets();

    // Add a new ticket after the fourth node
    ticketList.insertAfter(4, 6, "Sepitle", "Slow performance", "Open");

    // Remove the third ticket
    ticketList.removeTicket(3);

    // Print Modified List
    std::cout << "\nModified List:" << std::endl;
    ticketList.printAllTickets();

    // Display the number of tickets in the list
    std::cout << "\nNumber of tickets: " << ticketList.findLength() << std::endl;

    // Return and Print First Ticket
    Ticket* first = ticketList.returnFirst();
    if (first != nullptr) {
        std::cout << "\nFirst Ticket: " << first->ticketID << ", " << first->customerName << std::endl;
    }

    // Return and Print Last Ticket
    Ticket* last = ticketList.returnLast();
    if (last != nullptr) {
        std::cout << "Last Ticket: " << last->ticketID << ", " << last->customerName << std::endl;
    }

    // Find and Print Ticket with ID = 5
    Ticket* found = ticketList.findTicket(5);
    if (found != nullptr) {
        std::cout << "\nFound Ticket with ID 5: " << found->ticketID << ", " << found->customerName << std::endl;
    }

    return 0;
}
