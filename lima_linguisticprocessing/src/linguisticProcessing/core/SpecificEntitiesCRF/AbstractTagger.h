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
 * @file       AbstractTagger.h
 * @author     Grandjean Alex (alex.grandjean@cea.fr)
 * @date       Wed Mar 26 2014
 * copyright   Copyright (C) 20062014 by CEA LIST
 * Project     Lima
 * 
 * @brief      TODO
 * 
 * 
 ***********************************************************************/


#ifndef ABSTRACTTAGGER_H
#define ABSTRACTTAGGER_H

#include "SpecificEntitiesCRFExport.h"
#include "model.h"
#include "OutputTaggerWap.h"

#include <map>
#include <string>

namespace Lima
{
class AnalysisContent;

class AbstractTagger 
{

public:
  
  virtual ~AbstractTagger() {};

  virtual void initOptions(const std::map<std::string, std::string>& options) = 0;

  virtual OutputTaggerWap tag(AnalysisContent& analysis, MediaId lg) = 0;

  virtual void setMod(mdl_t *m)=0;

};
 
} // end namespace Lima
#endif
