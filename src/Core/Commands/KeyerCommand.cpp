#include "KeyerCommand.h"

#include <QtCore/QXmlStreamWriter>

KeyerCommand::KeyerCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

bool KeyerCommand::getDefer() const
{
    return this->defer;
}

void KeyerCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void KeyerCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void KeyerCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("defer", QString::number(this->getDefer()));
}
