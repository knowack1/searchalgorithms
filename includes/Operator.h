#ifndef OPERATOR_H_
#define OPERATOR_H_

#include "Node.h"

class Operator
{
public:
    virtual Node operator()(Node node)=0;
    virtual double weightScaler()
    {
        return 1;
    }
};

class OperatorLeft: public Operator
{
public:
    virtual Node operator()(Node node)
    {
        node.column -= glo_scale;
        return node;
    }
};

class OperatorRight: public Operator
{
public:
    virtual Node operator()(Node node)
    {
        node.column += glo_scale;
        return node;
    }
};

class OperatorUp: public Operator
{
public:
    virtual Node operator()(Node node)
    {
        node.row -= glo_scale;
        return node;
    }
};

class OperatorDown: public Operator
{
public:
    virtual Node operator()(Node node)
    {
        node.row += glo_scale;
        return node;
    }
};

class OperatorUpLeft: public OperatorUp, public OperatorLeft
{
public:
    virtual Node operator()(Node node)
    {
        node = OperatorUp::operator()(node);
        node = OperatorLeft::operator()(node);
        return node;
    }
    virtual double weightScaler(){  return sqrt(2);   }
};

class OperatorDownLeft: public OperatorDown, public OperatorLeft
{
public:
    virtual Node operator()(Node node)
    {
        node = OperatorDown::operator()(node);
        node = OperatorLeft::operator()(node);
        return node;
    }
    virtual double weightScaler(){  return sqrt(2);   }
};

class OperatorUpRight: public OperatorUp, public OperatorRight
{
public:
    virtual Node operator()(Node node)
    {
        node = OperatorUp::operator()(node);
        node = OperatorRight::operator()(node);
        return node;
    }
    virtual double weightScaler(){  return sqrt(2);   }
};

class OperatorDownRight: public OperatorDown, public OperatorRight
{
public:
    virtual Node operator()(Node node)
    {
        node = OperatorDown::operator()(node);
        node = OperatorRight::operator()(node);
        return node;
    }
    virtual double weightScaler(){  return sqrt(2);   }
};

#endif /* OPERATOR_H_ */
