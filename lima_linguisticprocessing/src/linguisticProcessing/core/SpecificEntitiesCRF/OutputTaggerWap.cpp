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
 
************************************************************************/
#include "OutputTaggerWap.h"
#include <iostream>

OutputTagWapData::OutputTagWapData() {

}

OutputTagWapData::~OutputTagWapData() {
}

OutputTagWapData::OutputTagWapData(uint64_t p, uint64_t l, std::string t, std::string s) {
  pos=p;
  lgth=l;
  type=t;
  str=s;

}
uint64_t OutputTagWapData::getPos() {
  return pos;
}

uint64_t OutputTagWapData::getLength() {
  return lgth;
}

std::string OutputTagWapData::getType() {
  return type;
}

std::string OutputTagWapData::getStr() {
  return str;
}


OutputTaggerWap::OutputTaggerWap() {

}

OutputTaggerWap::~OutputTaggerWap() {

}

int OutputTaggerWap::size() {
  return m_outputData.size();
}

void OutputTaggerWap::add(OutputTaggerWap outp) {

  for (int i=0; i<outp.size(); i++) {
    m_outputData.push_back(outp.m_outputData[i]);
  }

}

void OutputTaggerWap::setDataFromCrf(listDat_t* dat) {

  listDat_t *ltmp;
  ltmp=dat;
  while (ltmp!=NULL) {
    m_outputData.push_back(OutputTagWapData(ltmp->data->pos,ltmp->data->lgth,ltmp->data->type, ltmp->data->str));
    
    ltmp=ltmp->next;
  }
}
