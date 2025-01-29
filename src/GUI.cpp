
#pragma once
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtCore/QString>
#include <QtCore/QProcess>

class TerminalEmulator : public QWidget
{
public:
    TerminalEmulator(QWidget *parent = nullptr) : QWidget(parent)
    {
        // Set up the layout
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Create the output text area
        outputTextEdit = new QTextEdit(this);
        outputTextEdit->setReadOnly(true); // Make it read-only
        layout->addWidget(outputTextEdit);

        // Create the input line edit
        inputLineEdit = new QLineEdit(this);
        layout->addWidget(inputLineEdit);

        // Connect the input line edit to a lambda function that handles the input
        connect(inputLineEdit, &QLineEdit::returnPressed, [this]()
                {
            // Get the command from the input line edit
            QString command = inputLineEdit->text();
            inputLineEdit->clear();

            // Display the command in the output text area
            outputTextEdit->append("> " + command);

            // Execute the command using QProcess
            QProcess process;
            process.start(command);
            process.waitForFinished();

            // Read the output and error from the process
            QString output = process.readAllStandardOutput();
            QString error = process.readAllStandardError();

            // Display the output and error in the output text area
            if (!output.isEmpty()) {
                outputTextEdit->append(output);
            }
            if (!error.isEmpty()) {
                outputTextEdit->append("Error: " + error);
            } });

        // Apply dark theme using style sheets
        setStyleSheet(
            "QWidget { background-color: #2D2D2D; color: #FFFFFF; }"                              // Background and text color
            "QTextEdit { background-color: #1E1E1E; color: #FFFFFF; border: 1px solid #555555; }" // Output area
            "QLineEdit { background-color: #1E1E1E; color: #FFFFFF; border: 1px solid #555555; }" // Input field
        );

        // Set the layout to the widget
        setLayout(layout);

        // Set window properties
        setWindowTitle("Jewel Shell");
        setGeometry(100, 100, 600, 400);
    }

private:
    QTextEdit *outputTextEdit;
    QLineEdit *inputLineEdit;
};

int StartWindow(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create and show the terminal emulator
    TerminalEmulator terminal;
    terminal.show();

    // Execute the application
    return app.exec();
}