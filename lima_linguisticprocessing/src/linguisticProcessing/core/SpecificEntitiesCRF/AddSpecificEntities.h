/*
    Copyright 2002-2014 CEA LIST

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

 ************************************************************************
 *
 * @file       AddSpecificEntities.h
 * @author     Grandjean Alex (alex.grandjean@cea.fr)
 * @date       Fri Mar 06 2015
 * copyright   Copyright (C) 2006-2014 by CEA LIST
 * Project     Lima
 * 
 * @brief      Add the specific entities in the graph
 * 
 * 
 ***********************************************************************/

#ifndef ADDSPECIFICENTITIES_H
#define ADDSPECIFICENTITIES_H

#include "SpecificEntitiesCRFExport.h"
#include "AbstractCrfPostProcess.h"
#include "linguisticProcessing/core/Automaton/recognizer.h"

namespace Lima {

class AnalysisContent;
namespace LinguisticProcessing {
namespace LinguisticAnalysisStructure
{
  class AnalysisGraph;
}
namespace Automaton
{
  class RecognizerMatch;
}

  namespace SpecificEntitiesCRF {

#define ADDSPECIFICENTITIES_CLASSID "AddSpecificEntities"

class LIMA_SPECIFICENTITIESCRF_EXPORT AddSpecificEntities : public AbstractCrfPostProcess {

public:

    AddSpecificEntities();

    ~AddSpecificEntities();
    
   void init(
	     Common::XMLConfigurationFiles::GroupConfigurationStructure& unitConfiguration, Manager* manager)
  ;
    

   LimaStatusCode apply(AnalysisContent& analysis, MediaId lg, OutputTaggerWap& crfData) const;

void addSE(AnalysisContent& analysis, MediaId lg,  std::string str, std::string type, uint64_t pos, uint64_t length) const;

 void insertMap(std::string str, LinguisticProcessing::Automaton::ConstraintFunction* funct);

private:
    int m_style;
};

}
}
} // end namespace Lima

#endif
