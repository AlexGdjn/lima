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
 * @file       OutputTaggerWap.h
 * @author     Grandjean Alex (alex.grandjean@cea.fr)
 * @date       Fri Mar 28 2014
 * copyright   Copyright (C) 20062014 by CEA LIST
 * Project     Lima
 * 
 * @brief      Describe the datas given by the crf
 * 
 * 
 ***********************************************************************/
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
 * @file       OutputTaggerWap.h
 * @author     Grandjean Alex (alex.grandjean@cea.fr)
 * @date       Wed Mar 06 2014
 * copyright   Copyright (C) 20062014 by CEA LIST
 * Project     Lima
 * 
 * @brief      Describe the output datas from wapiti crf
 * 
 * 
 ***********************************************************************/
#ifndef OUTPUTTAGGERWAP_H
#define OUTPUTTAGGERWAP_H

#include <vector>
#include <string>
#include "decoder.h"

class OutputTagWapData {

public:
  OutputTagWapData();

  ~OutputTagWapData();

  OutputTagWapData(uint64_t p, uint64_t l, std::string t, std::string s);

  uint64_t getPos();

  uint64_t getLength();

  std::string getType();

  std::string getStr();

private:
  uint64_t pos;
  uint64_t lgth;
  std::string type;
  std::string str;

};


class OutputTaggerWap {

public:
  
  OutputTaggerWap();

  ~OutputTaggerWap();

  void setDataFromCrf(listDat_t* dat);

  /**
   * @brief Return the size of the OutputData vector 
   */
  int size();

  /**
   * @brief Add two OutputTaggerWap 
   */
  void add(OutputTaggerWap outp);

  std::vector<OutputTagWapData> m_outputData;

};
#endif
