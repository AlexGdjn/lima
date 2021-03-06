/*
Copyright 2002-2014 CEA LIST

This file is part of LIMA.

LIMA is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LIMA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with LIMA. If not, see <http://www.gnu.org/licenses/>
*/
/************************************************************************
*
* @file SemanticRoleLabelingLoader.h
* @author Clémence Filmont <clemence.filmont@cea.fr>
* @date 2014-04-17
* copyright Copyright (C) 2014 by CEA LIST
* Project mm_linguisticprocessing
*
* @brief an Semantic Role Labeling Loader class
*
*
***********************************************************************/

#ifndef SEMANTICROLELABELINGLOADER_H
#define SEMANTICROLELABELINGLOADER_H

#include "linguisticProcessing/core/LinguisticProcessors/AnalysisLoader.h"
#include "linguisticProcessing/core/LinguisticAnalysisStructure/AnalysisGraph.h"
#include "linguisticProcessing/core/LinguisticAnalysisStructure/LinguisticGraph.h"
#include <QFile>
#include <iostream>
#include <fstream>

namespace Lima {
namespace LinguisticProcessing {
namespace SemanticAnalysis {

#define SEMANTICROLELABELINGLOADER_CLASSID "SemanticRoleLabelingLoader"

class SemanticRoleLabelingLoader : public AnalysisLoader
{
 public:
  SemanticRoleLabelingLoader();
  virtual ~SemanticRoleLabelingLoader();

  void init(Common::XMLConfigurationFiles::GroupConfigurationStructure& unitConfiguration,
            Manager* manager);

  LimaStatusCode process(AnalysisContent& analysis) const;

 private:
  MediaId m_language;
  std::string m_graph;
  std::string m_suffix;
// QXmlSimpleReader* m_parser; /*< XML parser for the loader*/

  // XML handler
  class ConllHandler
  {
  public:
    QMap<uint64_t,LinguisticCode> m_tagIndex;

    ConllHandler(MediaId language, AnalysisContent& analysis, LinguisticAnalysisStructure::AnalysisGraph* graph);
    virtual ~ConllHandler();


    bool extractSemanticRole(const QString & expectedRole);// repeated on each line beginning



  private:
    MediaId m_language;
    AnalysisContent& m_analysis;
    LinguisticAnalysisStructure::AnalysisGraph* m_graph;
    uint64_t m_position;
    uint64_t m_length;
    std::string m_type;
    std::string m_string;
    std::string m_currentElement;

  };

};

} // end namespace
} // end namespace
} // end namespace

#endif