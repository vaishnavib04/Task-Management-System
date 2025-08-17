# GTK Todo App

A simple yet feature-rich desktop todo list application built with GTK+3 in C. This application provides a clean graphical interface for managing your daily tasks with persistent storage.

![GTK Todo App](https://via.placeholder.com/600x400/2E3440/ECEFF4?text=GTK+Todo+App)

## 🚀 Features

- **Add Tasks**: Create new tasks with a simple text input
- **Mark Complete**: Toggle task completion status with checkboxes
- **Edit Tasks**: Modify existing task titles through dialog boxes
- **Delete Tasks**: Remove unwanted tasks from the list
- **Visual Feedback**: Completed tasks are shown with strikethrough text
- **Persistent Storage**: Tasks are automatically saved to and loaded from `tasks.txt`
- **Clean UI**: Modern GTK+3 interface with scrollable task list
- **Responsive Design**: Properly sized window with organized layout

## 🛠️ Technical Details

### Architecture
- **Language**: C
- **GUI Framework**: GTK+3
- **Build System**: Make
- **Storage**: Plain text file (`tasks.txt`)
- **Platform**: Cross-platform (Windows, Linux, macOS)

### Key Components
- **Task Structure**: Each task has an ID, title, and completion status
- **File I/O**: Custom serialization format (`ID|Title|Completed`)
- **Memory Management**: Static array with maximum 100 tasks
- **Event Handling**: GTK signal system for UI interactions

### Data Format
Tasks are stored in `tasks.txt` using pipe-separated values:
```
ID|Task Title|Completion Status
```
Example:
```
1|Buy groceries|0
2|Complete project|1
3|Call dentist|0
```

## 📋 Requirements

### System Dependencies
- **GTK+3 development libraries**
- **GCC compiler**
- **pkg-config**
- **Make**

### Windows (MSYS2/MinGW)
```bash
pacman -S mingw-w64-x86_64-gtk3
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-pkg-config
pacman -S make
```

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install libgtk-3-dev gcc make pkg-config
```

### Fedora/RHEL
```bash
sudo dnf install gtk3-devel gcc make pkg-config
```

### macOS (Homebrew)
```bash
brew install gtk+3 gcc make pkg-config
```

## 🔧 Installation & Build

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/gtk-todo-app.git
   cd gtk-todo-app
   ```

2. **Build the application**
   ```bash
   make
   ```

3. **Run the application**
   ```bash
   ./build/todo-app
   ```

## 🎯 Usage

### Adding Tasks
1. Type your task in the text input field at the top
2. Click "Add Task" or press Enter
3. Your task will appear in the list below

### Managing Tasks
- **Complete**: Click the checkbox next to a task
- **Edit**: Click the "Edit" button to modify task text
- **Delete**: Click the "Delete" button to remove a task

### Data Persistence
- Tasks are automatically saved when you add, edit, delete, or complete them
- Data persists between application sessions
- Tasks are stored in `tasks.txt` in the application directory

## 📁 Project Structure

```
todo-app/
├── src/
│   └── main.c          # Main application source code
├── build/
│   └── todo-app.exe    # Compiled executable (after build)
├── tasks.txt           # Task storage file (created on first run)
├── Makefile            # Build configuration
└── README.md           # Project documentation
```

## 🔧 Build Configuration

The `Makefile` includes:
- **Compiler**: GCC with GTK+3 flags
- **Libraries**: GTK+3 linking with Windows subsystem support
- **Build Directory**: Automatic creation of `build/` folder
- **Clean Target**: Remove compiled binaries

## 🎨 UI Components

- **Main Window**: 450x500 pixels, centered on screen
- **Input Section**: Text entry field with "Add Task" button
- **Task List**: Scrollable list of tasks with individual controls
- **Task Items**: Checkbox, title label, Edit and Delete buttons
- **Edit Dialog**: Modal dialog for task title modification

## 🔄 Task Lifecycle

1. **Creation**: Task assigned unique ID and added to array
2. **Display**: Rendered in GTK list with appropriate styling
3. **Interaction**: User can complete, edit, or delete
4. **Persistence**: Changes immediately saved to file
5. **Reload**: Data restored on application restart

## 🐛 Error Handling

- **File I/O**: Graceful handling of missing or corrupt task files
- **Input Validation**: Empty task titles are rejected
- **Memory Limits**: Maximum 100 tasks supported
- **Parse Errors**: Invalid file entries are skipped with warnings

## 🚀 Future Enhancements

- [ ] Task categories and filtering
- [ ] Due dates and reminders
- [ ] Task priority levels
- [ ] Search functionality
- [ ] Export to different formats
- [ ] Dark/light theme toggle
- [ ] Keyboard shortcuts
- [ ] Task statistics and progress tracking

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 👨‍💻 Author

**Your Name**
- GitHub: [@yourusername](https://github.com/yourusername)
- Email: your.email@example.com

## 🙏 Acknowledgments

- GTK+ development team for the excellent GUI toolkit
- The C programming community for resources and examples
- Contributors and testers who helped improve this application

---

**Note**: Replace placeholder information (GitHub username, email, etc.) with your actual details before committing to your repository.
