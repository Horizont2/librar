#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

namespace Library {
    class Book {
    private:
        string title;
        string author;
        int year;
        string genre;
    public:

        Book(string title, string author, int year, string genre = "Unknown")
            : title(title), author(author), year(year), genre(genre) {}

        void setTitle(const string& newTitle) { title = newTitle; }
        void setAuthor(const string& newAuthor) { author = newAuthor; }
        void setYear(int newYear) { year = newYear; }
        void setGenre(const string& newGenre) { genre = newGenre; }

        string getTitle() const { return title; }
        string getAuthor() const { return author; }
        int getYear() const { return year; }
        string getGenre() const { return genre; }

        void display() const {
            cout << "Title: " << title
                 << ", Author: " << author
                 << ", Year: " << year
                 << ", Genre: " << genre << endl;
        }
    };
}

namespace Management {
    class LibraryManager {
    private:
        vector<Library::Book> books;
    public:

        void addBook(const Library::Book& book) {
            books.push_back(book);
            LOG_OPERATION("Added", book);
        }

        void removeBook(const string& title) {
            auto it = remove_if(books.begin(), books.end(), [&title](const Library::Book& book) {
                return book.getTitle() == title;
            });
            if (it != books.end()) {
                LOG_OPERATION("Removed", *it);
                books.erase(it, books.end());
            }
        }

        Library::Book* findBookByTitle(const string& title) {
            auto it = find_if(books.begin(), books.end(), [&title](const Library::Book& book) {
                return book.getTitle() == title;
            });
            return it != books.end() ? &(*it) : nullptr;
        }

        void listBooks() const {
            for (const auto& book : books) {
                book.display();
            }
        }

        void sortBooksBy(const string& criteria) {
            if (criteria == "title") {
                sort(books.begin(), books.end(), [](const Library::Book& a, const Library::Book& b) {
                    return a.getTitle() < b.getTitle();
                });
            } else if (criteria == "author") {
                sort(books.begin(), books.end(), [](const Library::Book& a, const Library::Book& b) {
                    return a.getAuthor() < b.getAuthor();
                });
            } else if (criteria == "year") {
                sort(books.begin(), books.end(), [](const Library::Book& a, const Library::Book& b) {
                    return a.getYear() < b.getYear();
                });
            }
        }
    };
}

#define LOG_OPERATION(operation, book)                            \
    {                                                             \
        ofstream logFile("library.log", ios::app);                \
        if (logFile.is_open()) {                                  \
            logFile << operation << ": "                          \
                    << book.getTitle() << " by " << book.getAuthor() \
                    << " (" << book.getYear() << ")\n";           \
            logFile.close();                                      \
        }                                                         \
    }

#define FIND_BOOK_BY_AUTHOR(manager, author)                     \
    {                                                            \
        cout << "Books by " << author << ":\n";                  \
        for (const auto& book : manager.books) {                 \
            if (book.getAuthor() == author) {                    \
                book.display();                                  \
            }                                                    \
        }                                                        \
    }

int main() {
    using namespace Library;
    using namespace Management;

    LibraryManager manager;

    Book book1("1984", "George Orwell", 1949, "Dystopian");
    Book book2("To Kill a Mockingbird", "Harper Lee", 1960, "Fiction");
    Book book3("Brave New World", "Aldous Huxley", 1932, "Science Fiction");

    manager.addBook(book1);
    manager.addBook(book2);
    manager.addBook(book3);

    cout << "All books in library:\n";
    manager.listBooks();

    manager.sortBooksBy("author");
    cout << "\nBooks sorted by author:\n";
    manager.listBooks();

    manager.removeBook("1984");
    cout << "\nAfter removing '1984':\n";
    manager.listBooks();

    FIND_BOOK_BY_AUTHOR(manager, "Harper Lee");

    return 0;
}
