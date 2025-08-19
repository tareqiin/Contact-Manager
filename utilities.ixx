module;

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

export module utilities;

namespace it_1 {
    namespace fs = std::filesystem;
    using namespace ftxui;

    const fs::path contacts_file = "contacts.txt";

    void ensure_file_exists() {
        if (!fs::exists(contacts_file)) {
            std::ofstream file(contacts_file);
        }
    }

    std::vector<std::string> read_contacts() {
        ensure_file_exists();
        std::vector<std::string> contacts;
        std::ifstream file(contacts_file);
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                contacts.push_back(line);
            }
        }
        return contacts;
    }

    void write_contacts(const std::vector<std::string>& contacts) {
        ensure_file_exists();
        std::ofstream file(contacts_file);
        for (const auto& contact : contacts) {
            file << contact << "\n";
        }
    }

    void add_contact(const std::string& name, const std::string& phone, const std::string& email) {
        ensure_file_exists();
        std::ofstream file(contacts_file, std::ios::app);
        file << name << "," << phone << "," << email << "\n";
    }

    void delete_contact(int index) {
        auto contacts = read_contacts();
        if (index >= 0 && index < (int)contacts.size()) {
            contacts.erase(contacts.begin() + index);
            write_contacts(contacts);
        }
    }

    export void app() {
    using namespace ftxui;

    ensure_file_exists();
    auto screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> contacts = read_contacts();
    int selected = 0; 

    std::string name_input; 
    std::string phone_input;
    std::string email_input;
    auto name_input_box = Input(&name_input, "Name");
    auto phone_input_box = Input(&phone_input, "Phone");
    auto email_input_box = Input(&email_input, "Email");

    // this one displaies the contacts in list 
    auto display_contacts = Menu(&contacts, &selected);

    auto add_button = Button("Add Contact", [&] {
        if (!name_input.empty() && !phone_input.empty() && !email_input.empty()) {
            add_contact(name_input, phone_input, email_input);
            contacts = read_contacts(); // Refresh contacts
            name_input.clear();
            phone_input.clear();
            email_input.clear();
        }
    });

    auto delete_button = Button("Delete Contact", [&] {
      if(!contacts.empty() && selected >= 0) {
        delete_contact(selected);
        contacts = read_contacts(); // Refresh contacts
        selected = 0;  // Adjust selection
      }
    });

    auto layout = Container::Vertical({
      name_input_box,
      phone_input_box,
      email_input_box,
      add_button,
      delete_button,
      display_contacts,
    }); 

    // this is my renderer which is goning to do a & function
   auto renderer = Renderer(layout, [&] {
    return vbox({
        text("Contact Manager") | bold | center,
        separator(),
        hbox({ text("Name:  "), name_input_box->Render() }),
        hbox({ text("Phone: "), phone_input_box->Render() }),
        hbox({ text("Email: "), email_input_box->Render() }),
        separator(),
        hbox({
            add_button->Render()    | color(Color::Blue) | flex,
            delete_button->Render() | color(Color::Red)  | flex,
        }) | center,
        separator(),
        text("Contacts:") | bold | center,
        display_contacts->Render() | flex,
    }) | border;
});

screen.Loop(renderer);

return; 
} // namespace it_1
}