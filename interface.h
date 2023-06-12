#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include <ctime>  //for time-delay
#include <cstdlib>  //for time delay
#include "task.h"
#include "taskManager.h"

void delay(int milliseconds) {
    std::clock_t start_time = std::clock();
    while ((std::clock() - start_time) / (double)CLOCKS_PER_SEC * 1000.0 < milliseconds);
}

class Interface {
private:
    TaskManager manager;    

public:

    Interface(){
        this->manager.dbFile = "taskManager.db";
        this->manager.openDatabase();
        this->manager.createTable();
    }

    void welcomeMessage() {
        std::cout << "------------------------------------------------------ WELCOME TO TASK MANAGER ------------------------------------------------------" << std::endl;
        delay(500);
        mainMenu();
    }
    
    void mainMenu() {
        std::cout << "MAIN MENU:" << std::endl;
        std::cout << "Select one of the options below:" << std::endl;
        delay(500);
        std::cout << "1. Display all tasks in the Task Manager" << std::endl;
        std::cout << "2. Add a new task" << std::endl;
        std::cout << "3. Modify an existing task" << std::endl;
        std::cout << "4. Delete an existing task" << std::endl;
        std::cout << "5. Find existing tasks" << std::endl;
        std::cout << "6. Delete all tasks" << std::endl;
        std::cout << "7. Quit the Task Manager" << std::endl;
        delay(500);
        choice();
    }

    int choice() {
        int choice;
        std::cout << "Your Choice: ";
        std::cin >> choice;
        std::getchar();
        switch (choice) {
            case 1: {   // for the "crosses initialization error"
                std::vector<Task> tasks;
                if (manager.getAllTasks(tasks)) manager.DisplayTaskDetails(tasks);
                else std::cout << "Error in displaying the tasks" << std::endl;
                std::cout << "\n";
                delay(500);
            }
            break;
            case 2: {
                Task t;
                t.createTask(t);
                if (manager.addTask(t)) std::cout << "Task created successfully!! Your Task Id is: " << t.id << std::endl;
                else std::cout << "Error in creating the task" << std::endl;
                std::cout << "\n";
                delay(500);
            }
            break;
            case 3: {
                int id;
                std::cout << "Enter the Task Id: ";
                std::cin >> id;
                std::getchar();
                if (updateTaskInterface(id)) std::cout << "Task Modified Successfully!!" << std::endl;
                else std::cout << "Error in modifying the task" << std::endl;
                std::cout << "\n";
                delay(500);
            }
            break;
            case 4: {
                int id, input;
                std::cout << "Enter the Task Id: ";
                std::cin >> id;
                std::getchar();
                std::cout << "Are you sure that you want to delete this task in the Task Manager. Enter 1 if YES otherwise press any other key to go back" << std::endl;
                std::cin >> input;
                std::getchar();
                if (input != 1) break;
                if (manager.deleteTask(id)) std::cout << "Task deleted successfully!!" << std::endl;
                else std::cout << "Error in deleting the task" << std::endl;
                std::cout << "\n";
                delay(500);
            }
            break;
            case 5: {
                int input;
                std::cout << "Select one of the options below: " << std::endl;
                delay(500);
                std::cout << "1. Find task by Id" << std::endl;
                std::cout << "2. Find task by Status" << std::endl;
                std::cout << "3. Find task by Priority" << std::endl;
                std::cout << "4. Find task by Starting Date" << std::endl;
                std::cout << "5. Find task by Due Date" << std::endl;
                std::cout << "Your Choice: ";
                std::cin >> input;
                std::getchar();
                switch (input) {
                    case 1: {
                        int id;
                        std::cout << "Enter the Task ID: ";
                        std::cin >> id;
                        std::getchar();
                        Task t;
                        if (manager.getTaskById(t, id)) t.DisplayTask();
                        std::cout << "\n\n";
                    }
                    break;
                    case 2: {
                        std::string status;
                        std::cout << "Enter the Task Status: ";
                        std::getline(std::cin, status);
                        std::vector<Task> t;
                        if (manager.getTasksByStatus(t, status)) manager.DisplayTaskDetails(t);
                        std::cout << "\n\n";
                    }
                    break;
                    case 3: {
                        int priority;
                        std::cout << "Enter the Task Priority: ";
                        std::cin >> priority;
                        std::getchar();
                        std::vector<Task> t;
                        if (manager.getTasksByPriority(t, priority)) manager.DisplayTaskDetails(t);
                        std::cout << "\n\n";
                    }
                    break;
                    case 4: {
                        std::string start;
                        std::cout << "Enter the Task Starting Date (in DD/MM/YYYY format): ";
                        std::getline(std::cin, start);
                        std::vector<Task> t;
                        if (manager.getTasksByStartDate(t, start)) manager.DisplayTaskDetails(t);
                        std::cout << "\n\n";
                    }
                    break;
                    case 5: {
                        std::string due;
                        std::cout << "Enter the Task Due Date (in DD/MM/YYYY format): ";
                        std::getline(std::cin, due);
                        std::vector<Task> t;
                        if (manager.getTasksByDueDate(t, due)) manager.DisplayTaskDetails(t);
                        std::cout << "\n\n";
                    }
                    break;
                    default: {
                        std::cout << "Incorrect Choice" << std::endl;
                        delay(500);
                        std::cout << "\n";
                    }
                    break;
                }
            }
            break;
            case 6: {
                int input;
                std::cout << "Are you sure that you want to delete all the tasks in the Task Manager. Press 1 if YES otherwise press any other key to go back" << std::endl;
                std::cin >> input;
                std::getchar();
                if (input != 1) break;
                else{
                    if (manager.deleteAllTasks()) std::cout << "All tasks deleted successfully!!" << std::endl;
                    else std::cout << "Error in deleting the tasks" << std::endl;
                    std::cout << "\n";
                    delay(500);
                }
            }
            break;
            case 7: {
                int input;
                std::cout << "Are you sure that you want to quit the Task Manager. Enter 1 if YES otherwise press any other key to go back" << std::endl;
                std::cin >> input;
                std::getchar();
                if (input == 1) return 0;
            }
            break;
            default: {
                std::cout << "Invalid option selected" << std::endl;
                std::cout << "\n";
            }
            break;
        }
        mainMenu();
        return 1;
    }

    bool updateTaskInterface(int taskId){
        std::string description, dueDate, startDate, status;
        int priority;
        std::cout << "Update the task Description: ";
        std::getline(std::cin, description); 
        std::cout << "Update the task Starting Date (DD/MM/YYYY): ";
        std::getline(std::cin, startDate);
        std::cout << "Update the task Due Date (DD/MM/YYYY): ";
        std::getline(std::cin, dueDate);
        std::cout << "Update the task Priority: ";
        std::cin >> priority;
        std::getchar();
        std::cout << "Update the task Status: ";
        std::getline(std::cin, status);
        return manager.updateTask(taskId, description, startDate, dueDate, priority, status);
    }
};

#endif 