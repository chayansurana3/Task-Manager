#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <iostream>
#include "sqlite3.h"
#include <string>
#include <vector>
#include "task.h"

class TaskManager : public Task{
private:


    bool dropTable() {
        std::string query = "DROP TABLE IF EXISTS TASKS";
        return executeQuery(query);
    }

public:
    std::string dbFile = "taskManager.db";
    sqlite3* db;

    TaskManager(){
        this->db = nullptr;
    }
    
    void openDatabase() {
        int result = sqlite3_open(this->dbFile.c_str(), &this->db);   //string of c style, i.e. null terminated 
        if (result != SQLITE_OK) {
            std::cerr << sqlite3_errmsg(this->db) << std::endl;
        }
    }

    void closeDatabase() {
        if (db) {
            int result = sqlite3_close(db);
            if (result != SQLITE_OK) {
                std::cerr << sqlite3_errmsg(db) << std::endl;
            }
            db = nullptr;
        }
    }

    bool executeQuery(std::string& query) {
        char* errorMessage = nullptr;
        int result = sqlite3_exec(this->db, query.c_str(), NULL, 0, &errorMessage);
        if (result != SQLITE_OK) {
            std::cerr << sqlite3_errmsg(this->db) << std::endl;
            sqlite3_free(errorMessage);
            return false;
        }
        return true;
    }
    
    void createTable() {
        std::string query = "CREATE TABLE IF NOT EXISTS TASKS ("
                            "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "Title TEXT NOT NULL,"
                            "Description TEXT,"
                            "Started_On TEXT,"
                            "Due_Date TEXT,"
                            "Priority INTEGER,"
                            "Status TEXT"
                            ")";
        executeQuery(query);
    }

    bool addTask(Task& task) {
        if (task.title.empty()) return false; // It is mandatory
        if (task.description.empty()) task.description = "NULL";
        if (task.startDate.empty()) task.startDate = "NULL";
        if (task.dueDate.empty()) task.dueDate = "NULL";
        if (task.status.empty()) task.status = "NULL";
        if (task.priority == -1) task.priority = 0;

        std::string query = "INSERT INTO TASKS (Title, Description, Started_On, Due_Date, Priority, Status) VALUES ('" +
                            task.title + "', '" + task.description + "', '" + task.startDate + "', '" + task.dueDate + "', " +
                            std::to_string(task.priority) + ", '" + task.status + "')";

        if (executeQuery(query)) {
            // Get the last inserted row id
            std::string lastInsertIdQuery = "SELECT last_insert_rowid()";
            sqlite3_stmt* statement;
            int result = sqlite3_prepare_v2(db, lastInsertIdQuery.c_str(), -1, &statement, nullptr);
            if (result != SQLITE_OK) {
                std::cerr << sqlite3_errmsg(db) << std::endl;
                return false;
            }
            if (sqlite3_step(statement) == SQLITE_ROW) {
                task.id = sqlite3_column_int(statement, 0);
            }
            sqlite3_finalize(statement);
            return true;
        }
        return false;
    }

    bool deleteTask(int taskId) {
        std::string query = "DELETE FROM TASKS WHERE id = " + std::to_string(taskId);
        return executeQuery(query);
    }

    bool deleteAllTasks() {
        std::string query = "DELETE FROM TASKS";
        return executeQuery(query);
    }

    bool updateTask(int taskId, std::string& description, std::string& startDate, std::string& dueDate, int priority, std::string& status) {
        std::string query = "UPDATE TASKS SET";
        bool hasSetClause = false;

        if (!description.empty()) {
            query += " Description = '" + description + "'";
            hasSetClause = true;
        }
        if (!startDate.empty()) {
            if (hasSetClause) query += ",";
            query += " Started_On = '" + startDate + "'";
            hasSetClause = true;
        }
        if (!dueDate.empty()) {
            if (hasSetClause) query += ",";
            query += " Due_Date = '" + dueDate + "'";
            hasSetClause = true;
        }
        if (priority != -1) {
            if (hasSetClause) query += ",";
            query += " Priority = " + std::to_string(priority);
            hasSetClause = true;
        }
        if (!status.empty()) {
            if (hasSetClause) query += ",";
            query += " Status = '" + status + "'";
            hasSetClause = true;
        }
        query += " WHERE id = " + std::to_string(taskId);
        return executeQuery(query);
    }

    bool getTaskById(Task& T, int taskId) {
        std::string query = "SELECT Id, Title, Description, Due_Date, Started_On, Priority, Status FROM TASKS WHERE Id = " + std::to_string(taskId);
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
        if (result != SQLITE_OK) {
            std::cout << "TASK NOT FOUND" << std::endl;
            return false;
        }
        if (sqlite3_step(statement) == SQLITE_ROW) {
            T.id = sqlite3_column_int(statement, 0);
            T.title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
            T.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
            T.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
            T.startDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
            T.priority = sqlite3_column_int(statement, 5);
            T.status = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
        }
        sqlite3_finalize(statement);
        return true;
    }

    bool getTasksByStatus(std::vector<Task>& tasks, std::string status) {
        std::string query = "SELECT Id, Title, Description, Due_Date, Started_On, Priority, Status FROM TASKS WHERE Status = '" + status + "'";
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
        if (result != SQLITE_OK) {
            std::cout << "TASK NOT FOUND" << std::endl;
            return false;
        }
        while (sqlite3_step(statement) == SQLITE_ROW) {
            Task T;
            T.id = sqlite3_column_int(statement, 0);
            T.title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
            T.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
            T.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
            T.startDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
            T.priority = sqlite3_column_int(statement, 5);
            T.status = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
            tasks.push_back(T);
        }
        sqlite3_finalize(statement);
        return true;
    }

    bool getTasksByPriority(std::vector<Task>& tasks, int priority) {
        std::string query = "SELECT Id, Title, Description, Due_Date, Started_On, Priority, Status FROM TASKS WHERE Priority = " + std::to_string(priority);
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
        if (result != SQLITE_OK) {
            std::cout << "TASK NOT FOUND" << std::endl;
            return false;
        }
        while (sqlite3_step(statement) == SQLITE_ROW) {
            Task T;
            T.id = sqlite3_column_int(statement, 0);
            T.title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
            T.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
            T.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
            T.startDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
            T.priority = sqlite3_column_int(statement, 5);
            T.status = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
            tasks.push_back(T);
        }
        sqlite3_finalize(statement);
        return true;
    }

    bool getTasksByDueDate(std::vector<Task>& tasks, std::string due_date) {
        std::string query = "SELECT Id, Title, Description, Due_Date, Started_On, Priority, Status FROM TASKS WHERE Due_Date = '" + due_date + "'";
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
        if (result != SQLITE_OK) {
            std::cout << "TASK NOT FOUND" << std::endl;
            return false;
        }
        while (sqlite3_step(statement) == SQLITE_ROW) {
            Task T;
            T.id = sqlite3_column_int(statement, 0);
            T.title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
            T.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
            T.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
            T.startDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
            T.priority = sqlite3_column_int(statement, 5);
            T.status = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
            tasks.push_back(T);
        }
        sqlite3_finalize(statement);
        return true;
    }

    bool getTasksByStartDate(std::vector<Task>& tasks, std::string start_date) {
        std::string query = "SELECT Id, Title, Description, Due_Date, Started_On, Priority, Status FROM TASKS WHERE Started_On = '" + start_date + "'";
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
        if (result != SQLITE_OK) {
            std::cout << "TASK NOT FOUND" << std::endl;
            return false;
        }
        while (sqlite3_step(statement) == SQLITE_ROW) {
            Task T;
            T.id = sqlite3_column_int(statement, 0);
            T.title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
            T.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
            T.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
            T.startDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
            T.priority = sqlite3_column_int(statement, 5);
            T.status = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
            tasks.push_back(T);
        }
        sqlite3_finalize(statement);
        return true;
    }

    bool getAllTasks(std::vector<Task>& tasks) {
        tasks.clear();
        std::string query = "SELECT * FROM TASKS";
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
        if (result != SQLITE_OK) {
            std::cerr << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        while (sqlite3_step(statement) == SQLITE_ROW) {
            Task task;
            task.id = sqlite3_column_int(statement, 0);
            task.title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
            task.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
            task.startDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
            task.dueDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
            task.priority = sqlite3_column_int(statement, 5);
            task.status = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
            tasks.push_back(task);
        }
        sqlite3_finalize(statement);
        return true;
    }

    void DisplayTaskDetails(std::vector<Task> &tasks){
        if (tasks.empty()){
            std::cout << "No Tasks Found in the Task Manager" << std::endl;
            return;
        }
        for (Task t : tasks) t.DisplayTask();
        std::cout << "\n\n";
    }

    ~TaskManager() {
        closeDatabase();
    }

};

#endif  