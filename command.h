#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <vector>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class CommandManager {
    std::vector<std::unique_ptr<Command>> commandHistory;
    int currentIndex = -1;

public:
    void execute(std::unique_ptr<Command> command) {
        commandHistory.resize(currentIndex + 1);
        command->execute();
        commandHistory.push_back(std::move(command));
        currentIndex++;
    }

    void undo() {
        if (canUndo()) {
            commandHistory[currentIndex--]->undo();
        }
    }

    void redo() {
        if (canRedo()) {
            commandHistory[++currentIndex]->execute();
        }
    }

    bool canUndo() const { return currentIndex >= 0; }
    bool canRedo() const { return currentIndex < static_cast<int>(commandHistory.size()) - 1; }
};

#endif // COMMAND_H