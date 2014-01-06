/***************************************************************************
 *   Copyright (C) 2011-2012 by Marcin Nawrocki <mar.nawrocki@gmail.com>   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "dict.h"
#include "ydptypes.h"
#include <QMap>
extern "C" {
#include "ydpdict/ydpdict.h"
}

typedef QMap<int, QStringList> DirectionToStringListMap;

class Dictionary : public IDictionary
{
public:
    Dictionary(int version, QString dictionaryPath);
    void release();
    bool isValid();
    const QStringList& entries(int direction);
    QString description(int direction, int index);
    int entryToIndex(int direction, QString entry);
private:
    bool open(int direction);
    void close();
    void initDirection(int direction);
    ydpdict_t* _dictData;
    DirectionToStringListMap _entries;
    int _version;
    QString _dictionaryPath;
public slots:
    void initialize(int version, QString dictionaryPath);
protected:
    ~Dictionary() {}
};

IDictionary* IDictionary::createInstance(int version, QString dictionaryPath)
{
    return new Dictionary(version, dictionaryPath);
}

Dictionary::Dictionary(int version, QString dictionaryPath):
    _dictData(nullptr),
    _version(version),
    _dictionaryPath(dictionaryPath)
{
    initDirection(YdpTypes::Foreign);
    initDirection(YdpTypes::Native);
}

void Dictionary::release()
{
    close();
    delete this;
}

bool Dictionary::isValid()
{
    return open(YdpTypes::Foreign) && open(YdpTypes::Native);
}

const QStringList& Dictionary::entries(int direction)
{
    return _entries[direction];
}

QString Dictionary::description(int direction, int index)
{
    if (open(direction))
        return QString::fromUtf8(ydpdict_read_xhtml(_dictData, index));

    return "";
}

int Dictionary::entryToIndex(int direction, QString entry)
{
    int index = -1;

    if (open(direction))
        while ((index = ydpdict_find_word(_dictData, entry.toUtf8())) == -1)
            entry.chop(1);

    return index;
}

void Dictionary::initialize(int version, QString dictionaryPath)
{
    if (version == _version)
    {
        _dictionaryPath = dictionaryPath;
        initDirection(YdpTypes::Foreign);
        initDirection(YdpTypes::Native);
    }
}

void Dictionary::initDirection(int direction)
{
    _entries[direction].clear();
    if (open(direction))
    {
        const int count = ydpdict_get_count(_dictData);
        for (int index=0; index<count; index++)
            _entries[direction].append(QString::fromUtf8(ydpdict_get_word(_dictData, index)));
    }
}

bool Dictionary::open(int direction)
{
    QString datFile = _dictionaryPath + QString("/dict%1.dat").arg(_version+direction);
    QString idxFile = _dictionaryPath + QString("/dict%1.idx").arg(_version+direction);

    close();
    _dictData = ydpdict_open(datFile.toAscii(), idxFile.toAscii(), YDPDICT_ENCODING_UTF8);
    if (!_dictData)
    {
        datFile = _dictionaryPath + QString("/dict%1.dat").toUpper().arg(_version+direction);
        idxFile = _dictionaryPath + QString("/dict%1.idx").toUpper().arg(_version+direction);
        _dictData = ydpdict_open(datFile.toAscii(), idxFile.toAscii(), YDPDICT_ENCODING_UTF8);
    }

    return _dictData;
}

void Dictionary::close()
{
    if (_dictData)
    {
        ydpdict_close(_dictData);
        _dictData = nullptr;
    }
}
