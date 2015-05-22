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
#include "AddSpecificEntities.h"

#include "linguisticProcessing/core/Automaton/recognizerData.h"
#include "linguisticProcessing/core/SpecificEntities/SpecificEntitiesConstraints.h"



using namespace std;
using namespace Lima::Common::AnnotationGraphs;
using namespace Lima::LinguisticProcessing::LinguisticAnalysisStructure;
using namespace Lima::LinguisticProcessing::Automaton;

namespace Lima
{

namespace LinguisticProcessing
{

namespace SpecificEntitiesCRF
{

  //SimpleFactory<MediaProcessUnit,AddSpecificEntities> normalizeDatePostProcessFactory(SPECIFICENTITIESCRFLEARN_CLASSID);

  AddSpecificEntities::AddSpecificEntities() {
  }

  AddSpecificEntities::~AddSpecificEntities() {
    std::map<std::string, LinguisticProcessing::Automaton::ConstraintFunction* >::const_iterator it;
    for (it=m_normalizeMap.begin(); it!=m_normalizeMap.end(); ++it) {
      delete (*it).second;
    }
  }
  


void AddSpecificEntities::init(
	     Common::XMLConfigurationFiles::GroupConfigurationStructure& unitConfiguration, Manager* manager)
{
}
  
  LimaStatusCode AddSpecificEntities::apply(AnalysisContent& analysis, MediaId lg, OutputTaggerWap& crfData) const
{
  LimaStatusCode lm;
  lm = SUCCESS_ID;

  bool isEN=false;
  int positionDebut=0;
  int lastPos=0;
  int lastLength=0;
  std::string currentEN;
  std::string currentStr;
  bool isFirst=true;
  
  if (crfData.m_outputData.size()>0) {
    for (uint64_t i=0; i<crfData.m_outputData.size(); ++i) {
      std::istringstream iss( crfData.m_outputData[i].getType()); 
      std::string mot; 
      std::vector<std::string> resvalue;
      while ( std::getline( iss, mot, '.' ) ) {
	resvalue.push_back(mot); 
      } 
      
      if (resvalue[1][0]=='B') {
	if (!isFirst) {
	  addSE(analysis,lg, currentStr , currentEN, positionDebut,(lastPos+lastLength)-positionDebut);
	} else {
	  isFirst=false;
	}
	positionDebut=crfData.m_outputData[i].getPos();
	lastPos=positionDebut;
	lastLength=crfData.m_outputData[i].getLength();
	resvalue[1].replace(0, 2, "");
	currentEN=resvalue[0]+"."+resvalue[1];
	currentStr=crfData.m_outputData[i].getStr();
	isEN=true;
      } else if (resvalue[1][0]=='I') {
	resvalue[1].replace(0, 2, "");
	if (resvalue[0]+"."+resvalue[1]!=currentEN || (crfData.m_outputData[i].getPos()>(lastPos+lastLength+1)) ) {
	  // if we don't start with a B-
	  if (!isFirst) {
	    addSE(analysis,lg, currentStr , currentEN, positionDebut,(lastPos+lastLength)-positionDebut);
	  } else {
	    isFirst=false;
	  }
	  currentEN=resvalue[0]+"."+resvalue[1];
	  currentStr=currentStr+" "+crfData.m_outputData[i].getStr();
	  positionDebut=crfData.m_outputData[i].getPos();
	  
	}
	lastPos=crfData.m_outputData[i].getPos();
	lastLength=crfData.m_outputData[i].getLength();
      } else {
      }
      
      
      
    }
    addSE(analysis,lg, currentStr , currentEN, positionDebut,(lastPos+lastLength)-positionDebut);
  }
 
  
  
  return lm;
}

  void AddSpecificEntities::addSE(AnalysisContent& analysis, MediaId lg,  std::string str, std::string type, uint64_t pos, uint64_t length) const {

 
AnalysisGraph* anagraph=static_cast<AnalysisGraph*>(analysis.getData("AnalysisGraph"));
//std::cout << "adding entity: type: " << type << ", str: " << str << ", pos: " << pos << ", length: " << length << std::endl;
  
  Lima::LinguisticProcessing::ApplyRecognizer::RecognizerData* recoData=new Lima::LinguisticProcessing::ApplyRecognizer::RecognizerData();
  
  analysis.setData("RecognizerData",recoData);
  Lima::LinguisticProcessing::ApplyRecognizer::RecognizerResultData* resultData=new Lima::LinguisticProcessing::ApplyRecognizer::RecognizerResultData("AnalysisGraph");
  recoData->setResultData(resultData);
 
  
  // create RecognizerMatch
  Automaton::RecognizerMatch match(anagraph);

  uint64_t posBegin=pos;
  uint64_t posEnd=posBegin+length;
  
  // find vertices corresponding to pos/len : have to go through the graph
  LinguisticGraph* graph=anagraph->getGraph();
  std::queue<LinguisticGraphVertex> toVisit;
  std::set<LinguisticGraphVertex> visited;
  LinguisticGraphOutEdgeIt outItr,outItrEnd;
 
  // output vertices between begin and end,
  toVisit.push(anagraph->firstVertex());
  
  bool first=true;
  bool inEntity=false;
  while (!toVisit.empty()) {
    LinguisticGraphVertex v=toVisit.front();
    toVisit.pop();
   
    if (v == anagraph->lastVertex()) {
      break;
    }

    if (first) {
      first=false;
    }
    else if (inEntity) {
      LinguisticAnalysisStructure::Token* t=get(vertex_token,*graph,v);
      if (t->position() >= posEnd) {
        inEntity=false;
        break; // no need to go further
      }
      else {
        match.addBackVertex(v);
      }
    }
    else {
      LinguisticAnalysisStructure::Token* t=get(vertex_token,*graph,v);
      if(t!=0) {
        if (t->position() == posBegin) {
	  
          match.addBackVertex(v);
          inEntity=true;
        }
      }
    }

    // add next vertices
    for (boost::tie(outItr,outItrEnd)=out_edges(v,*graph); outItr!=outItrEnd; outItr++) 
    {
      LinguisticGraphVertex next=target(*outItr,*graph);
      if (visited.find(next)==visited.end())
      {
        visited.insert(next);
        toVisit.push(next);
      }
    }
  }

  if (match.size()==0) {
    //LWARN << "Warning: no matching vertices for given position/length" << LENDL;
    //std::cout <<  "Warning: no matching vertices for given position/length" << std::endl;
  }
 
  // set entity properties
  match.setType(Common::MediaticData::MediaticData::single().getEntityType(Common::Misc::utf8stdstring2limastring(type)));
  // set normalized form similar to string (otherwise, may cause problem when trying to access the created specific entity)
  match.features().addFeature(DEFAULT_ATTRIBUTE,Common::Misc::utf8stdstring2limastring(str));
  
  // normalization
  bool res;
  std::map<std::string, LinguisticProcessing::Automaton::ConstraintFunction* >::const_iterator itF;
  itF=m_normalizeMap.find(type);
  if (itF!=m_normalizeMap.end()) {
    res=(*((*itF).second))(match, analysis);
  }

  // create specific entity from RecognizerMatch using default action
  Lima::LinguisticProcessing::SpecificEntities::CreateSpecificEntity createEntity(lg);
  createEntity(match,analysis);
  analysis.removeData("RecognizerData");
 
  delete resultData;
  }

void AddSpecificEntities::insertMap(std::string str, LinguisticProcessing::Automaton::ConstraintFunction* funct) {
  
  m_normalizeMap.insert(make_pair(str, funct));
}

}
}
}
