// ============================================================
// Library Catalog System
// Manages books, journals, and digital resources.
// Supports add, remove, search, and report operations via CLI.
// ============================================================

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

// Resource Class
class Resource {
public:
    enum class ResourceType {
        Book,
        Journal,
        Digital
    };

    Resource(const std::string& title, const std::string& author, const std::string& isbn, ResourceType type)
        : title(title), author(author), isbn(isbn), type(type) {}

    virtual ~Resource() = default;

    const std::string& getTitle() const { return title; }
    const std::string& getAuthor() const { return author; }
    const std::string& getISBN() const { return isbn; }
    ResourceType getType() const { return type; }

    virtual void display() const {
        std::cout << "Title: " << title << ", Author: " << author << ", ISBN: " << isbn << ", Type: " << getTypeName() << '\n';
    }

protected:
    std::string title;
    std::string author;
    std::string isbn;
    ResourceType type;

    std::string getTypeName() const {
        switch (type) {
            case ResourceType::Book: return "Book";
            case ResourceType::Journal: return "Journal";
            case ResourceType::Digital: return "Digital Resource";
            default: return "Unknown";
        }
    }
};

class Book : public Resource {
public:
    Book(const std::string& title, const std::string& author, const std::string& isbn)
        : Resource(title, author, isbn, ResourceType::Book) {}
};

class Journal : public Resource {
public:
    Journal(const std::string& title, const std::string& author, const std::string& isbn)
        : Resource(title, author, isbn, ResourceType::Journal) {}
};

class DigitalResource : public Resource {
public:
    DigitalResource(const std::string& title, const std::string& author, const std::string& isbn)
        : Resource(title, author, isbn, ResourceType::Digital) {}
};

// Catalog Class
class Catalog {
public:
    void addResource(std::unique_ptr<Resource> resource) {
        resources.push_back(std::move(resource));
    }

    void removeResource(const std::string& isbn) {
        resources.erase(std::remove_if(resources.begin(), resources.end(),
            [&isbn](const std::unique_ptr<Resource>& resource) {
                return resource->getISBN() == isbn;
            }), resources.end());
    }

    std::vector<Resource*> searchResources(const std::string& query) const {
        std::vector<Resource*> results;
        for (const auto& resource : resources) {
            if (resource->getTitle().find(query) != std::string::npos ||
                resource->getAuthor().find(query) != std::string::npos ||
                resource->getISBN().find(query) != std::string::npos) {
                results.push_back(resource.get());
            }
        }
        return results;
    }

    void generateReport() const {
        std::unordered_map<Resource::ResourceType, std::vector<Resource*>> groupedResources;

        for (const auto& resource : resources) {
            groupedResources[resource->getType()].push_back(resource.get());
        }

        for (const auto& entry : groupedResources) {
            Resource::ResourceType type = entry.first;
            const auto& resList = entry.second;

            std::cout << "Type: ";
            switch (type) {
                case Resource::ResourceType::Book: std::cout << "Book"; break;
                case Resource::ResourceType::Journal: std::cout << "Journal"; break;
                case Resource::ResourceType::Digital: std::cout << "Digital Resource"; break;
                default: std::cout << "Unknown"; break;
            }
            std::cout << '\n';

            for (const auto& res : resList) {
                res->display();
            }
            std::cout << '\n';
        }
    }

private:
    std::vector<std::unique_ptr<Resource>> resources;
};

// Command-Line Interface
void displayMenu() {
    std::cout << "Library Catalog System\n";
    std::cout << "1. Add Resource\n";
    std::cout << "2. Remove Resource\n";
    std::cout << "3. Search Resource\n";
    std::cout << "4. Generate Report\n";
    std::cout << "5. Exit\n";
}

int main() {
    Catalog catalog;
    int choice;

    while (true) {
        displayMenu();
        std::cout << "Choose an option: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::string title, author, isbn, type;
                std::cout << "Enter Title: ";
                std::getline(std::cin, title);
                std::cout << "Enter Author: ";
                std::getline(std::cin, author);
                std::cout << "Enter ISBN: ";
                std::getline(std::cin, isbn);
                std::cout << "Enter Type (book/journal/digital): ";
                std::getline(std::cin, type);
                if (type == "book") {
                    catalog.addResource(std::make_unique<Book>(title, author, isbn));
                } else if (type == "journal") {
                    catalog.addResource(std::make_unique<Journal>(title, author, isbn));
                } else if (type == "digital") {
                    catalog.addResource(std::make_unique<DigitalResource>(title, author, isbn));
                } else {
                    std::cout << "Invalid resource type.\n";
                }
                break;
            }
            case 2: {
                std::string isbn;
                std::cout << "Enter ISBN of resource to remove: ";
                std::getline(std::cin, isbn);
                catalog.removeResource(isbn);
                break;
            }
            case 3: {
                std::string query;
                std::cout << "Enter search query (title/author/ISBN): ";
                std::getline(std::cin, query);
                auto results = catalog.searchResources(query);
                for (const auto& res : results) {
                    res->display();
                }
                break;
            }
            case 4: {
                catalog.generateReport();
                break;
            }
            case 5:
                return 0;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
