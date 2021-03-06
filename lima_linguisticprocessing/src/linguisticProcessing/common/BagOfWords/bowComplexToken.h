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
/************************************************************************
 * @file     bowComplexToken.h
 * @author   Besancon Romaric
 * @date     Tue Oct  7 2003
 * copyright Copyright (C) 2003 by CEA LIST
 *
 * @brief
 *
 ***********************************************************************/

#ifndef BOWCOMPLEXTOKEN_H
#define BOWCOMPLEXTOKEN_H

#include "linguisticProcessing/LinguisticProcessingCommon.h"
#include "bowToken.h"

#include <boost/tuple/tuple.hpp>
#include <deque>

namespace Lima {
namespace Common {
namespace BagOfWords {



class BoWRelation;

class BoWComplexTokenPrivate;
/**
  * This is a complex token for an index. It is made of a collection of
  * BoWTokens's, one of which is the head of the token.
  *
  * @author Romaric Besancon
  */
class LIMA_BOW_EXPORT BoWComplexToken : public BoWToken
{
  friend class BoWComplexTokenPrivate;
public:

/**
   * This class represents a part of a ComplexToken : it is composed
   * of a pointer on the actual part (a BoWToken), and a boolean that
   * says if the BoWToken is also in the global list (the BoWText)
   * or not.
   */
  class Part : public boost::tuple<BoWRelation*,BoWToken*,bool>
  {
  public:
    Part():boost::tuple<BoWRelation*,BoWToken*,bool>(0,0,false) {};
    Part(BoWToken* tok, bool isInList):
      boost::tuple<BoWRelation*,BoWToken*,bool>(0,tok,isInList) {};
    Part(BoWRelation* rel, BoWToken* tok, bool isInList):
      boost::tuple<BoWRelation*,BoWToken*,bool>(rel,tok,isInList) {};
    BoWRelation* getBoWRelation() const { return get<0>(); }
    BoWToken* getBoWToken() const { return get<1>(); }
    bool isInList() const { return get<2>(); }
    void setInList(const bool b) { get<2>() = b; }
    LimaString getLemma() const { return get<1>()->getLemma(); }
    uint64_t getCategory() const { return get<1>()->getCategory(); }
  };

  BoWComplexToken();
//   /**
//    * copy constructor
//    *
//    * @attention the copy of the parts of a complex token is tricky :
//    * if a part of a complex token points to a token that have a proper
//    * existence outside of the part list, the pointer will be copied
//    * (no creation of a new object), otherwise a new object is created.
//    */
//   BoWComplexToken(const BoWComplexToken&);
// 
//   /**
//    * specialized copy constructor that takes into account a
//    * pointer-to-pointer map giving the correspondance of tokens for
//    * the component list, in case of a complete BoWText copy.
//    *
//    */
//   BoWComplexToken(const BoWComplexToken&,
//                   const std::map<BoWToken*,BoWToken*>&);

  BoWComplexToken(const Lima::LimaString& lemma,
                  const uint64_t category,
                  const uint64_t position,
                  const uint64_t length);

  /**
   * a constructor to build a simple, independent complex token
   * (parts are simple tokens, not listed elsewhere in a BoWText)
   *
   * @param lemma the lemma of the complex token
   * @param category the category of the complex token
   * @param position the position of the complex token
   * @param length the length of the complex token
   * @param parts a deque containing the simple tokens forming
   * the parts of the complex token
   * @param int head the index of the head token in the preceding list
   *
   * @return
   */
  BoWComplexToken(const Lima::LimaString& lemma,
                  const uint64_t category,
                  const uint64_t position,
                  const uint64_t length,
                  std::deque<BoWToken>& parts,
                  const uint64_t head);

  virtual ~BoWComplexToken();
  
//   virtual BoWComplexToken* clone() const;
//   virtual BoWComplexToken* clone(const std::map<BoWToken*,BoWToken*>&) const;

  virtual void clear();

  virtual Lima::LimaString getLemma(void) const;
  virtual Lima::LimaString getInflectedForm(void) const;

  BoWComplexToken& operator= (const BoWComplexToken&);
  bool operator== (const BoWComplexToken&);

  /** size is the number of parts in the token */
  uint64_t size(void) const;

  std::deque< Part >& getParts(void);
  const std::deque< Part >& getParts(void) const;

  std::deque< Part >::iterator getPartsIterator(void);
  const std::deque< Part >::const_iterator getPartsIterator(void) const;

  /**
   * add a part in the list of parts of the complex token. Have to
   * specify if the token forming the part exists as a proper token or
   * only as a part: in the first case, the pointer is added in the
   * part list, in the second case, a pointer on a copy of the token
   * is added in the list.
   *
   * @param tok a pointer on the BoWToken forming the part to add
   * @param isInList a boolean indicating, if true, that the BoWToken
   * pointed has an existence outside of the part list
   * @param isHead a boolean indicating that the BoWToken a
   */
  BoWToken* addPart(BoWToken* tok,
                    const bool isInList,
                    const bool isHead=false);

  /**
   * add a part in the list of parts of the complex token. Have to
   * specify if the token forming the part exists as a proper token or
   * only as a part: in the first case, the pointer is added in the
   * part list, in the second case, a pointer on a copy of the token
   * is added in the list.
   *
   * @param rel a pointer on the BoWRelation linking the part to add to the
   * head or the other parts
   * @param tok a pointer on the BoWToken forming the part to add
   * @param isInList a boolean indicating, if true, that the BoWToken
   * pointed has an existence outside of the part list
   * @param isHead a boolean indicating that the BoWToken a
   */
  BoWToken* addPart(BoWRelation* rel,
                    BoWToken* tok,
                    const bool isInList,
                    const bool isHead=false);

  /**
   * add a part in the list of parts of the complex token. A pointer on a copy of @ref tok
   * is added in the list.
   *
   * @param tok a pointer on the BoWToken forming the part to be copied and added
   * @param isHead a boolean indicating that the BoWToken is the head of the complex token it
   *        is part of
   */
  BoWToken* addPart(const BoWToken* tok,
//                     const bool isInList,
                    const bool isHead=false);
  
  /**
   * add a part in the list of parts of the complex token. A pointer on a copy of @ref tok
   * is added in the list.
   *
   * @param rel a pointer on the BoWRelation linking the part to copy and add to the
   * head or the other parts
   * @param tok a pointer on the BoWToken forming the part to be copied and added
   * @param isHead a boolean indicating that the BoWToken is the head of the complex token it
   *        is part of
   */
  BoWToken* addPart(const BoWRelation* rel,
                    const BoWToken* tok,
//                     const bool isInList,
                    const bool isHead=false);
  
  

  uint64_t getHead() const;
  void setHead(const uint64_t);

  // this is an abstract class, has no type
  virtual BoWType getType() const=0;

  // common function for output of derived classes
  std::string getUTF8StringParts(const Common::PropertyCode::PropertyManager* macroManager = 0) const;
  std::string getstdstringParts() const;

  virtual Common::Misc::PositionLengthList getPositionLengthList() const;

  /**
   * add the offset to the positions of the token
   * and of each of its parts (recusrively)
   *
   * @param offset the offset to add to all positions
   */
  void addToPosition(const uint64_t offset);

  
  /**
   * returns the vertices of all its parts
   *
   * @return the vertices of all its parts
   */
  virtual std::set< uint64_t > getVertices() const;

protected:
    BoWComplexToken(BoWComplexTokenPrivate& d);
};

  
} // namespace BagOfWords
} // namespace Common
} // namespace Lima

#endif
