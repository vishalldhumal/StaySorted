# StaySorted

**StaySorted** is a menu-driven C console application that helps hostel students organize assignments, everyday tasks, and spending.

## Features

- Add assignments with a subject, title, and due date.
- View assignments and mark them as complete.
- Add to-do tasks and mark them as done.
- Set a budget and record expenses by category.
- View a dashboard and reminders for upcoming deadlines.
- Save information in local files for later runs.

> Reminders appear inside the program. They are not phone or desktop notifications.

## Requirements

- A C compiler that supports C99, such as GCC
- Windows, macOS, or Linux terminal

## Compile and run

Open a terminal in the folder containing `hosteldesk_neat.c`.

### Windows PowerShell

```powershell
gcc -std=c99 -Wall -Wextra hosteldesk_neat.c -o hosteldesk_neat.exe
.\hosteldesk_neat.exe
```

### macOS or Linux

```bash
gcc -std=c99 -Wall -Wextra hosteldesk_neat.c -o hosteldesk_neat
./hosteldesk_neat
```

## How to use

1. Start the program from the terminal.
2. Choose a module from the main menu.
3. Follow the prompts to add or manage assignments, tasks, and expenses.
4. Choose the reminders option to view overdue assignments and upcoming deadlines.
5. Exit the program when finished. Keep its generated data files if you want your information to be available next time.

## C concepts demonstrated

- Structures and arrays
- Functions
- Loops and conditional statements
- Strings and input handling
- File handling
- Date and time handling

## Notes

- StaySorted is a local, single-user console program. It does not sync data between devices.
- Keep backup copies of the generated data files if you want to preserve your records.
- Avoid uploading personal assignment or spending data to a public repository.

## License

No license has been added. Add a `LICENSE` file if you want to specify how others may use or modify this project.
