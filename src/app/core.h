#ifndef DFRAMEFORKCORE_H
#define DFRAMEFORKCORE_H

///////////////////////////////////////////////////////////////////////////////
//
// Property system
//

#define D_PROPERTY_R(T, N, A) \
    A: T m_##N; \
    public: inline const T& get##N() const { return m_##N;};

#define D_PROPERTY_RW(T, N, A) \
    D_PROPERTY_R(T, N, A) \
    inline void set##N(const T& val) { m_##N = val; };

//#define D_PROPERTY_R(T, N)  D_PROPERTY_R(T, N, private)
//#define D_PROPERTY_RW(T, N) D_PROPERTY_RW(T, N, protected)
//#define D_PROPERTY(T, N)    D_PROPERTY_R(T, N)
#define D_PROPERTY(T, N) D_PROPERTY_R(T, N, private)
// Add in Qt property
#define DQ_PROPERTY_R(T, N, A) \
    D_PROPERTY_R(T, N, A) Q_PROPERTY(T N READ get##N)

#define DQ_PROPERTY_RW(T, N, A) \
    D_PROPERTY_RW(T, N, A) Q_PROPERTY(T N READ get##N WRITE set##N)

//#define DQ_PROPERTY_R(T, N)  DQ_PROPERTY_R(T, N, private)
//#define DQ_PROPERTY_RW(T, N) DQ_PROPERTY_RW(T, N, protected)
//#define DQ_PROPERTY(T, N)    DQ_PROPERTY_R(T, N)
#define DQ_PROPERTY(T, N, A) DQ_PROPERTY_R(T, N, A)

///////////////////////////////////////////////////////////////////////////////
//
// String defines
//

#define qStringConst(S) S.toLocal8Bit().constData()

///////////////////////////////////////////////////////////////////////////////
//
//  Memory defines
//

#define FREE_MEM(PTR) if(PTR == NULL) {delete PTR; PTR = NULL;}

#endif // DFRAMEFORKCORE_H
