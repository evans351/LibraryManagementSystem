// Online Library Management System using OOP in C++
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Book {
private:
    int id;
    std::string title;
    std::string author;
    bool isAvailable;

public:
    Book(int id, const std::string& title, const std::string& author)
        : id(id), title(title), author(author), isAvailable(true) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    bool available() const { return isAvailable; }

    void borrow() { isAvailable = false; }
    void returnBook() { isAvailable = true; }
};

class User {
private:
    int userId;
    std::string name;
    std::vector<int> borrowedBookIds;

public:
    User(int id, const std::string& name) : userId(id), name(name) {}

    int getUserId() const { return userId; }
    std::string getName() const { return name; }

    void borrowBook(int bookId) { borrowedBookIds.push_back(bookId); }

    void returnBook(int bookId) {
        borrowedBookIds.erase(std::remove(borrowedBookIds.begin(), borrowedBookIds.end(), bookId), borrowedBookIds.end());
    }

    std::vector<int> getBorrowedBooks() const { return borrowedBookIds; }
};

class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;

    Book* findBookById(int bookId) {
        for (auto& book : books) {
            if (book.getId() == bookId) return &book;
        }
        return nullptr;
    }

    User* findUserById(int userId) {
        for (auto& user : users) {
            if (user.getUserId() == userId) return &user;
        }
        return nullptr;
    }

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void removeBook(int bookId) {
        books.erase(std::remove_if(books.begin(), books.end(), [bookId](Book& b) { return b.getId() == bookId; }), books.end());
    }

    void registerUser(const User& user) {
        users.push_back(user);
    }

    void borrowBook(int userId, int bookId) {
        Book* book = findBookById(bookId);
        User* user = findUserById(userId);
        if (book && user && book->available()) {
            book->borrow();
            user->borrowBook(bookId);
            std::cout << user->getName() << " borrowed: " << book->getTitle() << "\n";
        } else {
            std::cout << "Cannot borrow book. Either not available or invalid user/book.\n";
        }
    }

    void returnBook(int userId, int bookId) {
        Book* book = findBookById(bookId);
        User* user = findUserById(userId);
        if (book && user) {
            book->returnBook();
            user->returnBook(bookId);
            std::cout << user->getName() << " returned: " << book->getTitle() << "\n";
        }
    }

    void searchBooksByTitle(const std::string& keyword) const {
        std::cout << "Search results for title containing '" << keyword << "':\n";
        for (const auto& book : books) {
            if (book.getTitle().find(keyword) != std::string::npos) {
                std::cout << book.getId() << ": " << book.getTitle() << " by " << book.getAuthor() << (book.available() ? " [Available]\n" : " [Borrowed]\n");
            }
        }
    }

    void listAllBooks() const {
        std::cout << "Listing all books:\n";
        for (const auto& book : books) {
            std::cout << book.getId() << ": " << book.getTitle() << " by " << book.getAuthor() << (book.available() ? " [Available]\n" : " [Borrowed]\n");
        }
    }
};

int main() {
    Library lib;

    lib.addBook(Book(1, "C++ Fundamentals", "Bjarne Stroustrup"));
    lib.addBook(Book(2, "Clean Code", "Robert C. Martin"));
    lib.addBook(Book(3, "The Pragmatic Programmer", "Andy Hunt"));

    lib.registerUser(User(101, "Alice"));
    lib.registerUser(User(102, "Bob"));

    lib.listAllBooks();

    lib.borrowBook(101, 2);
    lib.borrowBook(102, 2); // Should fail

    lib.searchBooksByTitle("Code");

    lib.returnBook(101, 2);
    lib.borrowBook(102, 2); // Should succeed

    lib.listAllBooks();

        std::cout << "\n--- Test: Returning a book that wasn't borrowed (Fail) ---" << std::endl;
    lib.returnBook(102, 3); // Bob never borrowed book 3

    std::cout << "\n--- Test: Returning a book twice (Fail on second return) ---" << std::endl;
    lib.returnBook(102, 2); // Bob returns successfully
    lib.returnBook(102, 2); // Try again — should have no effect or fail silently

    std::cout << "\n--- Test: Borrowing a non-existent book (Fail) ---" << std::endl;
    lib.borrowBook(101, 99); // Book ID 99 does not exist

    std::cout << "\n--- Test: Borrowing with invalid user (Fail) ---" << std::endl;
    lib.borrowBook(999, 1); // User ID 999 does not exist

    std::cout << "\n--- Test: Searching for non-existent title ---" << std::endl;
    lib.searchBooksByTitle("Quantum Physics");

    std::cout << "\n--- Final Book List ---" << std::endl;
    lib.listAllBooks();

    std::cout << "\n--- End of Library Management System ---" << std::endl;

    return 0;
}
