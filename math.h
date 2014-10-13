#ifndef MATH_H
#define MATH_H

#include <QVector3D>

struct IndexedFingerForThumbOrdering{
    QVector3D position;
    QVector3D thumb;
};


    /**
    * @brief Returns true if the x coordinate from QVector3D v1 is greater than the x coordinate of QVector3D v2
    * Neaded for sorting all fingers (each finger is a QVector3D) of the left hand.
    * @param v1 Vector of type QVector3D
    * @param v2 Vector of type QVector3D
    * @return bool
    */
   inline bool vectorXGreaterThan(const QVector3D &v1, const QVector3D &v2){

       return v1.x() > v2.x();
   }

   /**
    * @brief Returns true if the x coordinate from QVector3D v1 is smaller than the x coordinate of QVector3D v2.
    * Neaded for sorting all fingers (each finger is a QVector3D) of the right hand.
    * @param v1 Vector of type QVector3D
    * @param v2 Vector of type QVector3D
    * @return bool
   */
   inline bool vectorXLessThan(const QVector3D &v1, const QVector3D &v2){
       return v1.x() < v2.x();
   }


   /**
    * @brief Calculates the center of a circle in a 3D Space. Needs three different points as parameter.
    * @param Point 1
    * @param Point 2
    * @param Point 3
    * @return The center of type QVector3D
    */
   inline QVector3D calculateCircleCenter(QVector3D p1, QVector3D p2, QVector3D p3){
       QVector3D t = p2 - p1;
       QVector3D u = p3 - p1;
       QVector3D v = p3 - p2;

       // Helfer
       double nenner = 2.0 * (QVector3D::crossProduct((p1 - p2), (p2 - p3))).lengthSquared();


       double a = v.lengthSquared() * QVector3D::dotProduct((p1 - p2), (p1 - p3)) / nenner;
       double b = u.lengthSquared() * QVector3D::dotProduct((p2 - p1), (p2 - p3)) / nenner;
       double g = t.lengthSquared() * QVector3D::dotProduct((p3 - p1), (p3 - p2)) / nenner;

       return a * p1 + b * p2 + g * p3;
   }



   /**
    * @brief Calculates the radius of a circle based on three points of type QVector3D
    * @param p1
    * @param p2
    * @param p3
    * @return the radius of the circle as double
    */
   inline double calculateCircleRadius(QVector3D p1, QVector3D p2, QVector3D p3){
       QVector3D t = p2 - p1;
       QVector3D u = p3 - p1;
       QVector3D v = p3 - p2;

       // Formel von Wikipedia
       double radius = (t.length() * v.length() * u.length()) /
           (2 * (QVector3D::crossProduct(t, v)).length());

       return radius;
   }

   /**
    * @brief Returns true if the x coordinate from QVector3D v1 is smaller than the x coordinate of QVector3D v2.
    * Neaded for sorting all fingers (each finger is a QVector3D).
    * @param v1 Vector of type QVector3D
    * @param v2 Vector of type QVector3D
    * @return bool
    */
    inline bool distanceLowerToThumb(IndexedFingerForThumbOrdering finger1, IndexedFingerForThumbOrdering finger2){
        return finger1.position.distanceToPoint(finger1.thumb) < finger2.position.distanceToPoint(finger2.thumb);
    }


#endif // MATH_H
