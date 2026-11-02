# HostelDesk

HostelDesk is a menu-driven C console application that helps hostel students organize assignments, everyday tasks, and spending.

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
gcc -std=c99 -Wall -Wextra hosteldesk_neat.c -o hosteldesk
./hosteldesk
```

## C concepts demonstrated

- Structures and arrays
- Functions
- Loops and conditional statements
- Strings and input handling
- File handling
- Date and time handling

## Notes

- HostelDesk is a local, single-user console program. It does not sync data between devices.
- Keep the data files created by the program if you want to retain your saved information.
- Don’t upload personal expense or assignment data to a public repository.

## License

Add a `LICENSE` file if you want to specify how others may use or modify this project.