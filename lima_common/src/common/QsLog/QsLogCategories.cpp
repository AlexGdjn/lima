/*
    Copyright 2002-2013 CEA LIST

    This file is part of LIMA.

    LIMA is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LIMA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with LIMA.  If not, see <http://www.gnu.org/licenses/>
*/
#include "QsLogCategories.h"
#include "common/misc/LimaFileSystemWatcher.h"

#ifdef WIN32
#pragma warning(disable: 4127)
#endif
#include <QFile>
#include <QList>
#include <QDateTime>
#include <QtGlobal>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <cassert>
#include <cstdlib>
#include <stdexcept>

using namespace Lima;

namespace QsLogging
{

// static const int init =  initQsLog();
  
class CategoriesImpl
{
public:
   CategoriesImpl()
   {
   }
   QMap<QString,Level> categories;

   LimaFileSystemWatcher m_configFileWatcher;
};

Categories::Categories(QObject* parent) :
  QObject(parent),
  d(new CategoriesImpl())
{
  connect(&d->m_configFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(configureFileChanged(QString)));
}

Categories::~Categories()
{
   delete d;
}

void Categories::configureFileChanged ( const QString & path )
{
  if (QFile(path).exists())
  {
    configure(path);
  }
}

bool Categories::configure(const QString& fileName)
{
  QFile file(fileName);
  QFileInfo fileInfo(fileName);
  QDir configDir = fileInfo.dir();

  if (configDir.exists("log4cpp"))
  {
    QString log4cppSubdirName = configDir.filePath("log4cpp");
    QFileInfo log4cppSubdirInfo(log4cppSubdirName);
    if (log4cppSubdirInfo.isDir())
    {
      QStringList nameFilters;
      nameFilters << "log4cpp.*.properties";
      QDir log4cppSubdir(log4cppSubdirName);
      QFileInfoList configFiles = log4cppSubdir.entryInfoList(nameFilters);
      Q_FOREACH(QFileInfo configFile, configFiles)
      {
        configure(configFile.absoluteFilePath());
      }
    }
  }

  if (!file.open(QIODevice::ReadOnly))
  {
    std::cerr << "Unable to open qslog configuration file: " << fileName.toUtf8().data() << std::endl;
    return false;
  }
  d->m_configFileWatcher.addPath(fileName);

  bool res = true;
  QTextStream in(&file);
  QString line = in.readLine();
  while (!line.isNull())
  {
    if (!line.startsWith("#"))
    {
      QStringList elts = line.split("=");
      if (elts.size()==2 && elts.at(0).trimmed().startsWith("log4j.category."))
      {
        QString category = elts.at(0).trimmed().remove(0,QString("log4j.category.").size());
        QString value = elts.at(1).trimmed();
        if (value == "TRACE")
          d->categories.insert(category,QsLogging::TraceLevel);
        else if (value == "DEBUG")
          d->categories.insert(category,QsLogging::DebugLevel);
        else if (value == "INFO")
          d->categories.insert(category,QsLogging::InfoLevel);
        else if (value == "WARN")
          d->categories.insert(category,QsLogging::WarnLevel);
        else if (value == "ERROR")
          d->categories.insert(category,QsLogging::ErrorLevel);
        else if (value == "FATAL")
          d->categories.insert(category,QsLogging::FatalLevel);
        else
        {
          std::cerr << "Error reading " << fileName.toUtf8().constData() << ": unknow level " << value.toUtf8().constData() << ". Using TRACE" << std::endl;
          res = false;
          d->categories.insert(category,QsLogging::TraceLevel);
        }
      }
      else if (elts.size()==2 && elts.at(0).trimmed() == "include")
      {
        QString includedFileName = elts.at(1).trimmed();
        QString includedInitFileName = includedFileName;
        if  (!QFileInfo(includedInitFileName).isAbsolute())
        {
          includedInitFileName = configDir.filePath(includedInitFileName);
        }
        configure(includedInitFileName);
      }
    }
    line = in.readLine();
  }
  return res;
}

Level Categories::levelFor(const QString& category) const
{
  if (d->categories.contains(category))
  {
    return d->categories[category];
  }
  std::cerr << "Error: unknown category. Using TRACE for " << category.toUtf8().constData() << std::endl;
  return QsLogging::TraceLevel;
}

LIMA_COMMONQSLOG_EXPORT int initQsLog() {
  try {
    QString initFileName = QString::fromUtf8(qgetenv("LIMA_CONF").isEmpty()?"/usr/share/config/lima":qgetenv("LIMA_CONF").constData())+ "/log4cpp.properties";
    if (!QsLogging::Categories::instance().configure(initFileName))
    {
      std::cerr << "Configure Problem " << initFileName.toUtf8().constData() << std::endl;
      return -1;
    }
    //    }
} catch(...) {
  std::cerr << "Exception during logging system configuration" << std::endl;
  return -1;
}
return 0;
}

} // end namespace

