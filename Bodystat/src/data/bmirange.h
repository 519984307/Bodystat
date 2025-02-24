﻿#ifndef BMI_RANGE_H
#define BMI_RANGE_H

class QString;
class QSqlQuery;

class BmiRange{
public:
    typedef enum{
        SEX_UNKNOWN=0,
        SEX_MALE=1,
        SEX_FEMAL=2,
        SEX_OTHER=3,
    }Sex;
public:
    BmiRange();
    explicit BmiRange(const BmiRange &range);
    ~BmiRange();
public:
    int pull(QSqlQuery &query);
    int isValid(QString *errMsg=0) const;
    int isContain(const unsigned int age,
        const unsigned int sex) const;
    void setAgeLower(const unsigned int lower);
    unsigned int getAgeLower() const;
    QString getAgeLowerText() const;
    void setAgeUpper(const unsigned int upper);
    unsigned int getAgeUpper() const;
    QString getAgeUpperText() const;
    void setSex(const unsigned int sex);
    unsigned int getSex() const;
    QString getSexText() const;
    void setReferLower(const float lower);
    float getReferLower() const;
    QString getReferLowerText() const;
    void setReferUpper(const float upper);
    float getReferUpper() const;
    QString getReferUpperText() const;
public:
    BmiRange &operator=(const BmiRange &range);
private:
    BmiRange(
        const unsigned int ageLower,
        const unsigned int ageUpper,
        const unsigned int sex,
        const float referLower,
        const float referUpper);
private:
    unsigned int _ageLower;
    unsigned int _ageUpper;
    unsigned int _sex;
    float _referLower;
    float _referUpper;
};

#endif // BMI_RANGE_H
