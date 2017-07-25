#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QVector>

struct Command
{
    unsigned short in_1;
    unsigned short in_2;
    unsigned short out;
    QChar type;
    unsigned short number;
    QString data;
    Command(){data = "";}
};

class Parser
{
public:
    Parser();
    int parse_command_string(QString data, QVector<Command *> &command_list);
};

#endif // PARSER_H
