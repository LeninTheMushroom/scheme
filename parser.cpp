#include "parser.h"

Parser::Parser()
{

}

int Parser::parse_command_string(QString data, QVector<Command*> &command_list)
{
    if(data.size() == 0)
        return -1;
    if(data.at(data.size()-1) != '.')
        return -2;
    QString processed_data = data;
    processed_data.remove(' ');
    processed_data.chop(1);
    int string_iterator = 0;
    while(string_iterator < processed_data.size())
    {
        int old_position = string_iterator;
        while(string_iterator < processed_data.size() && processed_data.at(string_iterator) != ';')
            string_iterator++;
        QString command_string = processed_data.mid(old_position, string_iterator-old_position);
        Command *instance = new Command;
        instance->in_1 = command_string.at(0).digitValue();
        instance->in_2 = command_string.at(1).digitValue();
        instance->out = command_string.at(2).digitValue();
        instance->type = command_string.at(3);
        instance->number = command_string.at(4).digitValue();
        if(command_string.size()>5)
        {
            if(command_string.at(5) == '=')
                instance->data = command_string.mid(6);
            else
                return -3;
        }
        command_list.push_back(instance);
        string_iterator++;
    }
    return 1;
}
