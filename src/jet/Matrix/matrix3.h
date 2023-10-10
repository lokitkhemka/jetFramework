#pragma once

#include<Matrix/matrix.h>
#include<Vector/vector3.h>
#include<array>
#include<limits>

namespace jet
{
    //!\brief 3 by 3 matrix class
    //!
    //! This is a row-major 3 by 3 matrix class, in which the elements are stored
    //! in row-order form.
    //! \tparam T Type of the element
    //!
    template<typename T>
    class Matrix<T, 3, 3>
    {
    public:
        static_assert(std::is_floating_point<T>::value, "Matrix can only be instantiated with float point values.");

        //! Constructs Identity Matrix
        Matrix();

        //! Constructs a constant value matrix
        explicit Matrix(T s);

        //! Constructs a matrix with input elements. The ordering of the elements is row-major
        Matrix ( T m00, T m01, T m02, 
                T m10, T m11, T m12,
                T m20, T m21, T m22);

        //! \brief Constructs a matrix with given initializer list \p list.
        //!
        //! This constructor will build a matrix with given initializer list \p list
        //! such as
        //!
        //! \code{.cpp}
        //! Matrix<int, 3, 3> arr = {
        //!     {1, 2, 4},
        //!     {9, 3, 5},
        //!     {4, 8, 1}
        //! };
        //! \endcode
        //!
        //! Note the initializer also has 2x2 structure.
        //!
        //! \param list Initializer list that should be copy to the new matrix.
        //!
        template <typename U>
        Matrix(const std::initializer_list<std::initializer_list<U>>& lst);

        //! Constructs a matrix with input matrix
        Matrix(const Matrix& m);

        //! Constructs a matrix with input array. The ordering of the elements will be row-major
        explicit Matrix(const T* arr);

        //! Sets the whole matrix with input scalar.
        void Set(T s);

        //! Sets the matrix with input elements in row-major ordering
        void Set(T m00, T m01, T m02, 
                T m10, T m11, T m12,
                T m20, T m21, T m22);

        //! \brief Sets this matrix with given initializer list \p list.
        //!
        //! This function will fill the matrix with given initializer list \p list
        //! such as
        //!
        //! \code{.cpp}
        //! Matrix<int, 3, 3> arr = {
        //!     {1, 2, 4},
        //!     {9, 3, 5},
        //!     {4, 8, 1}
        //! };
        //! \endcode
        //!
        //! Note the initializer also has 2x2 structure.
        //!
        //! \param list Initializer list that should be copy to the matrix.
        //!
        template <typename U>
        void Set(const std::initializer_list<std::initializer_list<U>>& list);

        //! Copies from input matrix.
        void Set(const Matrix& m);

        //! Copies from input array in row-major order
        void Set(const T* arr);

        //! Sets diagonal elements with input scalar.
        void SetDiagonal(T s);

        //! Sets off diagonal elements with input scalar.
        void SetOffDiagonal(T s);

        //! Set \p i th row with input vector \p row.
        void SetRow(size_t i, const Vector3<T>& row);

        //! Set \p i th column with input vector \p col.
        void SetCol(size_t i, const Vector3<T>& col);

        
        
        //! Returns true if this matrix is similar to the input matrix within the given tolerance
        bool IsSimilar(const Matrix& m, double tol = std::numeric_limits<double>::epsilon()) const;

        //! Returns true if the matrix is square
        bool IsSquare() const;

        //! Returns Number of rows of this matrix
        size_t Rows() const;

        //! Returns the number of columns of this matrix
        size_t Cols() const;

        //! Returns data pointer of this matrix
        T* Data();

        //! Returns the const pointer of the matrix
        const T* Data() const;


        //! Returns this matrix + input scalar
        Matrix Add(T s) const;

        //! Returns this Matrix + Input matrix (element-wise)
        Matrix Add(const Matrix& m) const;

        //! Returns this matrix - input scalar
        Matrix Sub(T s) const;

        //! Returns this matrix - input matrix (element-wise)
        Matrix Sub(const Matrix& m) const;

        //! Returns this matrix * input scalar.
        Matrix Mul(T s) const;

        //! Returns this matrix * input vector.
        Vector3<T> Mul(const Vector3<T>& v) const;

        //! Returns this matrix * input matrix.
        Matrix Mul(const Matrix& m) const;

        //! Returns this matrix / input scalar.
        Matrix Div(T s) const;

        
        //! Returns input scalar + this matrix
        Matrix RAdd(T s) const;

        //! Returns Input scalar - this matrix.
        Matrix RSub(T s) const;

        //! Returns Input Matrix - this matrix (element-wise)
        Matrix RSub(const Matrix& m) const;

        //! Returns Input Scalar * this matrix
        Matrix RMul(T s) const;

        //! Returns input matrix * this matrix.
        Matrix RMul(const Matrix& m) const;

        //! Returns input scalar / this matrix.
        Matrix RDiv(T s) const;




        //! Adds Input Sclar to this matrix
        void IAdd(T s);

        //! Adds Input Matrix to This Matrix (element-wise)
        void IAdd(const Matrix& m);

        //! Subtracts Input Scalar from this matrix
        void ISub(T s);

        //! Subtracts Input Matrix from this matrix.
        void ISub(const Matrix& m);

        //! Multiplies Input Scalar from this matrix.
        void IMul(T s);

        //! Multiplies Input Matrix to this matrix.
        void IMul(const Matrix& m);

        //! Divides this matrix with input scalar
        void IDiv(T s);



        //! Transposes this matrix.
        void Transpose();

        //! Inverts this matrix.
        void Invert();

        
        //! Returns the sum of all elements.
        T Sum() const;

        //! Returns the average of all elements.
        T Avg() const;

        //! Returns the minimum among all elements.
        T Min() const;

        //! Returns the maximum among all the elements.
        T Max() const;

        //! Returns the absolute minimum among all the elements.
        T AbsMin() const;

        //! Returns the absolute maximum among all the elements.
        T AbsMax() const;

        //! Returns the sum of all diagonal elements.
        T Trace() const;

        //! Returns the determinant of this matrix.
        T Determinant() const;

        //! Returns the diagonal part of this matrix.
        Matrix Diagonal() const;

        //! Returns the off-diagonal part of this matrix.
        Matrix OffDiagonal() const;

        //! Returns the strict Lower Triangle part of this matrix.
        Matrix StrictLowerTri() const;

        //! Returns the strictly Upper triangle part of this matrix.
        Matrix StrictUpperTri() const;

        //! Returns Lower Triangle Part of this matrix, including the diagonal.
        Matrix LowerTri() const;

        //! Returns Upper Triangle Part of this matrix, including the diagonal.
        Matrix UpperTri() const;

        //! Returns Transposed Matrix.
        Matrix Transposed() const;

        //! Returns the Inverse matrix.
        Matrix Inverse() const;

        template<typename U>
        Matrix<U,3,3> CastTo() const;


        //! Assigns the Input Matrix.
        Matrix& operator=( const Matrix& m);

        //! Addition assignment with input scalar.
        Matrix& operator+=(T s);

        //! Addition assignment with input matrix
        Matrix& operator+=(const Matrix& m);

        //! Subtraction assignment with input scalar.
        Matrix& operator-=(T s);

        //! Subtraction assignment with input matrix(element wise)
        Matrix& operator-=(const Matrix& m);

        //! Multiplication assignment with input scalar.
        Matrix& operator*=(T s);

        //! Multiplication assigment with input matrix.
        Matrix& operator*=(const Matrix& m);

        //! Division assignment with input scalar.
        Matrix& operator/=(T s);

        

        //! Returns the reference to the \p i-th element.
        T& operator[](size_t i);

        //! Returns the const reference to the \p i-th element.
        const T& operator[](size_t i) const;

        //! Returns the reference to (i,j) element.
        T& operator()(size_t i, size_t j);

        //! Returns the const reference to (i,j) element.
        const T& operator()(size_t i, size_t j) const;

        //! Returns true if this matrix is equal to \p m.
        bool operator==(const Matrix& m) const;

        //! Returns true if this matrix is not equal to \p m.
        bool operator!=(const Matrix& m) const;



        //! Sets all the matrix entries to zero.
        static Matrix MakeZero();

        //! Converts the matrix into identity matrix.
        static Matrix MakeIdentity();

        //! Makes Scale Matrix.
        static Matrix MakeScaleMatrix(T sx, T sy, T sz);

        //! Makes scale matrix.
        static Matrix MakeScaleMatrix(const Vector3<T>& s);

        //! Makes Rotation Matrix. \p rad is rotation angle in radians
        static Matrix MakeRotationMatrix(const Vector3<T>& axis, T rad);

    private:
        std::array<T,9> _elements;
    };

    //! Type alias for 2x2 matrix.
    template<typename T> using Matrix3x3 = Matrix<T,3,3>;

    //! Returns the matrix with opposite sign
    template<typename T>
    Matrix3x3<T> operator-(const Matrix3x3<T>& a);

    //! Returns a + b (element-wise)
    template<typename T>
    Matrix3x3<T> operator+(const Matrix3x3<T>& a, const Matrix3x3<T>& b);

    //! Returns the a + b, where b is a scalar. The sum is element-wise
    template<typename T>
    Matrix3x3<T> operator+(const Matrix3x3<T>& a, const T& b);

    //! Returns the a + b, where a is a scalar. The sum is element-wise
    template<typename T>
    Matrix3x3<T> operator+(const T& a, const Matrix3x3<T>& b);

    //! Returns a-b element-wise
    template<typename T>
    Matrix3x3<T> operator-(const Matrix3x3<T>& a, const Matrix3x3<T>& b);

    //! Returns a-b, where b is a scalar. The difference is element-wise
    template<typename T>
    Matrix3x3<T> operator-(const Matrix3x3<T>& a, T b);

    //! Returns a-b, where a is a scalar. The difference is element-wise
    template<typename T>
    Matrix3x3<T> operator-(T a, const Matrix3x3<T>& b);

    //! Returns a * b, where b is a input scalar. The product is element-wise
    template<typename T>
    Matrix3x3<T> operator*(const Matrix3x3<T>& a, T b);

    //! Returns a * b, where a is an input scalar. The product is element-wise
    template<typename T>
    Matrix3x3<T> operator*(T a, const Matrix3x3<T>& b);

    //! Returns a * b
    template<typename T>
    Vector3<T> operator*(const Matrix3x3<T>& a, const Vector3<T>& b);

    //! Returns a * b
    template<typename T>
    Matrix3x3<T> operator*(const Matrix3x3<T>& a, const Matrix3x3<T>& b);

    //! Returns a / b, where b is a scalar. The division is element-wise.
    template<typename T>
    Matrix3x3<T> operator/(const Matrix3x3<T>& a, T b);

    //! Returns a / b, where a is a scalar. The division is element-wise.
    template<typename T>
    Matrix3x3<T> operator/(T a, const Matrix3x3<T>& b);

    //! Float-type 2x2 matrix.
    typedef Matrix3x3<float> Matrix3x3F;

    //! Double-type 2x2 matrix.
    typedef Matrix3x3<double> Matrix3x3D;


    /*
    * IMPLEMENTATIONS OF METHODS DECLARED ABOVE.
    */
    template<typename T>
    Matrix3x3<T>::Matrix()
    {
        Set(1, 0, 0,
            0, 1, 0,
            0, 0, 1);
    }

    template<typename T>
    Matrix3x3<T>::Matrix(T s)
    {
        Set(s);
    }


    template<typename T>
    Matrix3x3<T>::Matrix(T m00, T m01, T m02,
                            T m10, T m11, T m12,
                            T m20, T m21, T m22)
    {
        Set(m00, m01, m02,
            m10, m11, m12,
            m20, m21, m22);
    }
    
    template <typename T>
    template <typename U>
    Matrix3x3<T>::Matrix(
        const std::initializer_list<std::initializer_list<U>>& lst) {
        Set(lst);
    }

    template<typename T>
    Matrix3x3<T>::Matrix(const Matrix& m)
    {
        Set(m);
    }

    template<typename T>
    Matrix3x3<T>::Matrix(const T* arr)
    {
        Set(arr);
    }



    template<typename T>
    void Matrix3x3<T>::Set(T s)
    {
        _elements[0] = _elements[1] = _elements[2] = _elements[3] =
        _elements[4] = _elements[5] = _elements[6] = _elements[7] =
        _elements[8]= s;
    }

    template <typename T>
void Matrix<T, 3, 3>::Set(
    T m00, T m01, T m02,
    T m10, T m11, T m12,
    T m20, T m21, T m22) {
    _elements[0] = m00;
    _elements[1] = m01;
    _elements[2] = m02;
    _elements[3] = m10;
    _elements[4] = m11;
    _elements[5] = m12;
    _elements[6] = m20;
    _elements[7] = m21;
    _elements[8] = m22;
}

    template <typename T>
    template <typename U>
    void Matrix<T, 3, 3>::Set(
        const std::initializer_list<std::initializer_list<U>>& lst) {
        size_t height = lst.size();
        size_t width = (height > 0) ? lst.begin()->size() : 0;
        JET_ASSERT(width == 3);
        JET_ASSERT(height == 3);

        auto rowIter = lst.begin();
        for (size_t i = 0; i < height; ++i) {
            JET_ASSERT(width == rowIter->size());
            auto colIter = rowIter->begin();
            for (size_t j = 0; j < width; ++j) {
                (*this)(i, j) = static_cast<T>(*colIter);
                ++colIter;
            }
            ++rowIter;
        }
    }

    template <typename T>
    void Matrix3x3<T>::Set(const Matrix& m) {
        memcpy(_elements.data(), m._elements.data(), sizeof(T) * 9);
    }

    template <typename T>
    void Matrix3x3<T>::Set(const T* arr) {
        memcpy(_elements.data(), arr, sizeof(T) * 9);
    }

    template<typename T>
    void Matrix3x3<T>::SetDiagonal(T s)
    {
        _elements[0] = _elements[4] = _elements[8] = s;
    }

    template<typename T>
    void Matrix3x3<T>::SetOffDiagonal(T s)
    {
        _elements[1] = _elements[2] = _elements[3] =
        _elements[5] = _elements[6] = _elements[7] = s;
    }

    template <typename T>
    void Matrix3x3<T>::SetRow(size_t i, const Vector<T, 3>& row) {
        _elements[3 * i] = row.x;
        _elements[3 * i + 1] = row.y;
        _elements[3 * i + 2] = row.z;
    }

    template <typename T>
    void Matrix3x3<T>::SetCol(size_t j, const Vector<T, 3>& col) {
        _elements[j] = col.x;
        _elements[j + 3] = col.y;
        _elements[j + 6] = col.z;
    }

    

    template<typename T>
    bool Matrix3x3<T>::IsSimilar(const Matrix& m, double tol) const
    {
        return ((std::fabs(_elements[0] - m._elements[0]) < tol) &&
                (std::fabs(_elements[1] - m._elements[1]) < tol) &&
                (std::fabs(_elements[2] - m._elements[2]) < tol) &&
                (std::fabs(_elements[3] - m._elements[3]) < tol) &&
                (std::fabs(_elements[4] - m._elements[4]) < tol) &&
                (std::fabs(_elements[5] - m._elements[5]) < tol) &&
                (std::fabs(_elements[6] - m._elements[6]) < tol) &&
                (std::fabs(_elements[7] - m._elements[7]) < tol) &&
                (std::fabs(_elements[8] - m._elements[8]) < tol));
    }

    template<typename T>
    bool Matrix3x3<T>::IsSquare() const
    {
        return true;
    }

    template <typename T>
    size_t Matrix3x3<T>::Rows() const {
        return 3;
    }

    template <typename T>
    size_t Matrix3x3<T>::Cols() const {
        return 3;
    }

    template <typename T>
    T* Matrix3x3<T>::Data() {
        return _elements.data();
    }

    template <typename T>
    const T* Matrix3x3<T>::Data() const {
        return _elements.data();
    }



    template <typename T>
    Matrix3x3<T> Matrix3x3<T>::Add(T s) const {
        return Matrix(
             _elements[0] + s, _elements[1] + s, _elements[2] + s,
             _elements[3] + s, _elements[4] + s, _elements[5] + s,
             _elements[6] + s, _elements[7] + s, _elements[8] + s);
    }

    template <typename T>
    Matrix3x3<T> Matrix3x3<T>::Add(const Matrix& m) const {
        return Matrix(
            _elements[0] + m._elements[0],
            _elements[1] + m._elements[1],
            _elements[2] + m._elements[2],
            _elements[3] + m._elements[3],
            _elements[4] + m._elements[4],
            _elements[5] + m._elements[5],
            _elements[6] + m._elements[6],
            _elements[7] + m._elements[7],
            _elements[8] + m._elements[8]);
    }

    template <typename T>
    Matrix3x3<T> Matrix3x3<T>::Sub(T s) const {
        return Matrix(
            _elements[0] - s, _elements[1] - s, _elements[2] - s,
            _elements[3] - s, _elements[4] - s, _elements[5] - s,
            _elements[6] - s, _elements[7] - s, _elements[8] - s
            );
    }

    template <typename T>
    Matrix3x3<T> Matrix3x3<T>::Sub(const Matrix& m) const {
        return Matrix(
            _elements[0] - m._elements[0],
            _elements[1] - m._elements[1],
            _elements[2] - m._elements[2],
            _elements[3] - m._elements[3],
            _elements[4] - m._elements[4],
            _elements[5] - m._elements[5],
            _elements[6] - m._elements[6],
            _elements[7] - m._elements[7],
            _elements[8] - m._elements[8]);
    }

    template <typename T>
    Matrix3x3<T> Matrix3x3<T>::Mul(T s) const {
        return Matrix(
            _elements[0] * s, _elements[1] * s, _elements[2] * s,
            _elements[3] * s, _elements[4] * s, _elements[5] * s,
            _elements[6] * s, _elements[7] * s, _elements[8] * s);
    }

    template <typename T>
    Vector<T, 3> Matrix<T, 3, 3>::Mul(const Vector<T, 3>& v) const {
        return Vector<T, 3>(
            _elements[0] * v.x + _elements[1] * v.y + _elements[2] * v.z,
            _elements[3] * v.x + _elements[4] * v.y + _elements[5] * v.z,
            _elements[6] * v.x + _elements[7] * v.y + _elements[8] * v.z);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::Mul(const Matrix& m) const {
        return Matrix(
            _elements[0] * m._elements[0]
        + _elements[1] * m._elements[3]
        + _elements[2] * m._elements[6],
            _elements[0] * m._elements[1]
        + _elements[1] * m._elements[4]
        + _elements[2] * m._elements[7],
            _elements[0] * m._elements[2]
        + _elements[1] * m._elements[5]
        + _elements[2] * m._elements[8],

            _elements[3] * m._elements[0]
        + _elements[4] * m._elements[3]
        + _elements[5] * m._elements[6],
            _elements[3] * m._elements[1]
        + _elements[4] * m._elements[4]
        + _elements[5] * m._elements[7],
            _elements[3] * m._elements[2]
        + _elements[4] * m._elements[5]
        + _elements[5] * m._elements[8],

            _elements[6] * m._elements[0]
        + _elements[7] * m._elements[3]
        + _elements[8] * m._elements[6],
            _elements[6] * m._elements[1]
        + _elements[7] * m._elements[4]
        + _elements[8] * m._elements[7],
            _elements[6] * m._elements[2]
        + _elements[7] * m._elements[5]
        + _elements[8] * m._elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::Div(T s) const {
        return Matrix(
            _elements[0] / s, _elements[1] / s, _elements[2] / s,
            _elements[3] / s, _elements[4] / s, _elements[5] / s,
            _elements[6] / s, _elements[7] / s, _elements[8] / s);
    }




    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::RAdd(T s) const {
        return Matrix(
            s + _elements[0], s + _elements[1], s + _elements[2],
            s + _elements[3], s + _elements[4], s + _elements[5],
            s + _elements[6], s + _elements[7], s + _elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::RSub(T s) const {
        return Matrix(
            s - _elements[0], s - _elements[1], s - _elements[2],
            s - _elements[3], s - _elements[4], s - _elements[5],
            s - _elements[6], s - _elements[7], s - _elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::RSub(const Matrix& m) const {
        return Matrix(
            m._elements[0] - _elements[0],
            m._elements[1] - _elements[1],
            m._elements[2] - _elements[2],
            m._elements[3] - _elements[3],
            m._elements[4] - _elements[4],
            m._elements[5] - _elements[5],
            m._elements[6] - _elements[6],
            m._elements[7] - _elements[7],
            m._elements[8] - _elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::RMul(T s) const {
        return Matrix(
            s * _elements[0], s * _elements[1], s * _elements[2],
            s * _elements[3], s * _elements[4], s * _elements[5],
            s * _elements[6], s * _elements[7], s * _elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::RMul(const Matrix& m) const {
        return m.Mul(*this);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::RDiv(T s) const {
        return Matrix(
            s / _elements[0], s / _elements[1], s / _elements[2],
            s / _elements[3], s / _elements[4], s / _elements[5],
            s / _elements[6], s / _elements[7], s / _elements[8]);
    }





    template <typename T>
    void Matrix<T, 3, 3>::IAdd(T s) {
        _elements[0] += s;
        _elements[1] += s;
        _elements[2] += s;
        _elements[3] += s;
        _elements[4] += s;
        _elements[5] += s;
        _elements[6] += s;
        _elements[7] += s;
        _elements[8] += s;
    }

    template <typename T>
    void Matrix<T, 3, 3>::IAdd(const Matrix& m) {
        _elements[0] += m._elements[0];
        _elements[1] += m._elements[1];
        _elements[2] += m._elements[2];
        _elements[3] += m._elements[3];
        _elements[4] += m._elements[4];
        _elements[5] += m._elements[5];
        _elements[6] += m._elements[6];
        _elements[7] += m._elements[7];
        _elements[8] += m._elements[8];
    }

    template <typename T>
    void Matrix<T, 3, 3>::ISub(T s) {
        _elements[0] -= s;
        _elements[1] -= s;
        _elements[2] -= s;
        _elements[3] -= s;
        _elements[4] -= s;
        _elements[5] -= s;
        _elements[6] -= s;
        _elements[7] -= s;
        _elements[8] -= s;
    }

    template <typename T>
    void Matrix<T, 3, 3>::ISub(const Matrix& m) {
        _elements[0] -= m._elements[0];
        _elements[1] -= m._elements[1];
        _elements[2] -= m._elements[2];
        _elements[3] -= m._elements[3];
        _elements[4] -= m._elements[4];
        _elements[5] -= m._elements[5];
        _elements[6] -= m._elements[6];
        _elements[7] -= m._elements[7];
        _elements[8] -= m._elements[8];
    }

    template <typename T>
    void Matrix<T, 3, 3>::IMul(T s) {
        _elements[0] *= s;
        _elements[1] *= s;
        _elements[2] *= s;
        _elements[3] *= s;
        _elements[4] *= s;
        _elements[5] *= s;
        _elements[6] *= s;
        _elements[7] *= s;
        _elements[8] *= s;
    }

    template <typename T>
    void Matrix<T, 3, 3>::IMul(const Matrix& m) {
        Set(Mul(m));
    }

    template <typename T>
    void Matrix<T, 3, 3>::IDiv(T s) {
        _elements[0] /= s;
        _elements[1] /= s;
        _elements[2] /= s;
        _elements[3] /= s;
        _elements[4] /= s;
        _elements[5] /= s;
        _elements[6] /= s;
        _elements[7] /= s;
        _elements[8] /= s;
    }




    template <typename T>
    void Matrix<T, 3, 3>::Transpose() {
        std::swap(_elements[1], _elements[3]);
        std::swap(_elements[2], _elements[6]);
        std::swap(_elements[5], _elements[7]);
    }

    template <typename T>
    void Matrix<T, 3, 3>::Invert() {
        T d = Determinant();

        Matrix m;
        m._elements[0] = _elements[4] * _elements[8] - _elements[5] * _elements[7];
        m._elements[1] = _elements[2] * _elements[7] - _elements[1] * _elements[8];
        m._elements[2] = _elements[1] * _elements[5] - _elements[2] * _elements[4];
        m._elements[3] = _elements[5] * _elements[6] - _elements[3] * _elements[8];
        m._elements[4] = _elements[0] * _elements[8] - _elements[2] * _elements[6];
        m._elements[5] = _elements[2] * _elements[3] - _elements[0] * _elements[5];
        m._elements[6] = _elements[3] * _elements[7] - _elements[4] * _elements[6];
        m._elements[7] = _elements[1] * _elements[6] - _elements[0] * _elements[7];
        m._elements[8] = _elements[0] * _elements[4] - _elements[1] * _elements[3];
        m.IDiv(d);

        Set(m);
    }




    template<typename T>
    T Matrix3x3<T>::Sum() const
    {
        T s = 0;
        for (int i = 0; i < 9; ++i)
            s += _elements[i];
        return s;
    }

    template<typename T>
    T Matrix3x3<T>::Avg() const
    {
        return Sum()/9;
    }

    template<typename T>
    T Matrix3x3<T>::Min() const
    {
        return MinN(Data(), 9);
    }

    template<typename T>
    T Matrix3x3<T>::Max() const
    {
        return MaxN(Data(), 9);
    }

    template<typename T>
    T Matrix3x3<T>::AbsMin() const
    {
        return AbsMinN(Data(), 9);
    }

    template<typename T>
    T Matrix3x3<T>::AbsMax() const
    {
        return AbsMaxN(Data(), 9);
    }

    template<typename T>
    T Matrix3x3<T>::Trace() const
    {
        return _elements[0] + _elements[4] + _elements[8];
    }

    template <typename T>
T Matrix<T, 3, 3>::Determinant() const {
    return
        _elements[0] * _elements[4] * _elements[8]
      - _elements[0] * _elements[5] * _elements[7]
      + _elements[1] * _elements[5] * _elements[6]
      - _elements[1] * _elements[3] * _elements[8]
      + _elements[2] * _elements[3] * _elements[7]
      - _elements[2] * _elements[4] * _elements[6];
}





    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::Diagonal() const {
        return Matrix(
            _elements[0], 0, 0,
            0, _elements[4], 0,
            0, 0, _elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::OffDiagonal() const {
        return Matrix(
            0, _elements[1], _elements[2],
            _elements[3], 0, _elements[5],
            _elements[6], _elements[7], 0);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::StrictLowerTri() const {
        return Matrix(
            0, 0, 0,
            _elements[3], 0, 0,
            _elements[6], _elements[7], 0);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::StrictUpperTri() const {
        return Matrix(
            0, _elements[1], _elements[2],
            0, 0, _elements[5],
            0, 0, 0);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::LowerTri() const {
        return Matrix(
            _elements[0], 0, 0,
            _elements[3], _elements[4], 0,
            _elements[6], _elements[7], _elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::UpperTri() const {
        return Matrix(
            _elements[0], _elements[1], _elements[2],
            0, _elements[4], _elements[5],
            0, 0, _elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::Transposed() const {
        return Matrix(
            _elements[0], _elements[3], _elements[6],
            _elements[1], _elements[4], _elements[7],
            _elements[2], _elements[5], _elements[8]);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::Inverse() const {
        Matrix m(*this);
        m.Invert();
        return m;
    }



    template <typename T>
    template <typename U>
    Matrix<U, 3, 3> Matrix<T, 3, 3>::CastTo() const {
        return Matrix<U, 3, 3>(
            static_cast<U>(_elements[0]),
            static_cast<U>(_elements[1]),
            static_cast<U>(_elements[2]),
            static_cast<U>(_elements[3]),
            static_cast<U>(_elements[4]),
            static_cast<U>(_elements[5]),
            static_cast<U>(_elements[6]),
            static_cast<U>(_elements[7]),
            static_cast<U>(_elements[8]));
    }




    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix& m)
    {
        Set(m);
        return *this;
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator+=(T s)
    {
        IAdd(s);
        return *this;
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix& m)
    {
        IAdd(m);
        return *this;
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator-=(T s)
    {
        ISub(s);
        return *this;
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix& m)
    {
        ISub(m);
        return *this;
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator*=(T s)
    {
        IMul(s);
        return *this;
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix& m)
    {
        IMul(m);
        return *this;
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator/=(T s)
    {
        IDiv(s);
        return *this;
    }

    template <typename T>
    bool Matrix<T, 3, 3>::operator==(const Matrix& m) const {
        return
            _elements[0] == m._elements[0]
        && _elements[1] == m._elements[1]
        && _elements[2] == m._elements[2]
        && _elements[3] == m._elements[3]
        && _elements[4] == m._elements[4]
        && _elements[5] == m._elements[5]
        && _elements[6] == m._elements[6]
        && _elements[7] == m._elements[7]
        && _elements[8] == m._elements[8];
    }

    template <typename T>
    bool Matrix<T, 3, 3>::operator!=(const Matrix& m) const {
        return
            _elements[0] != m._elements[0]
        || _elements[1] != m._elements[1]
        || _elements[2] != m._elements[2]
        || _elements[3] != m._elements[3]
        || _elements[4] != m._elements[4]
        || _elements[5] != m._elements[5]
        || _elements[6] != m._elements[6]
        || _elements[7] != m._elements[7]
        || _elements[8] != m._elements[8];
    }





    template <typename T>
    T& Matrix<T, 3, 3>::operator[](size_t i) {
        return _elements[i];
    }

    template <typename T>
    const T& Matrix<T, 3, 3>::operator[](size_t i) const {
        return _elements[i];
    }

    template <typename T>
    T& Matrix<T, 3, 3>::operator()(size_t i, size_t j) {
        return _elements[3 * i + j];
    }

    template <typename T>
    const T& Matrix<T, 3, 3>::operator()(size_t i, size_t j) const {
        return _elements[3 * i + j];
    }


    
    
    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::MakeZero() {
        return Matrix(
            0, 0, 0,
            0, 0, 0,
            0, 0, 0);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::MakeIdentity() {
        return Matrix(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::MakeScaleMatrix(T sx, T sy, T sz) {
        return Matrix(
            sx, 0, 0,
            0, sy, 0,
            0, 0, sz);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::MakeScaleMatrix(const Vector<T, 3>& s) {
        return MakeScaleMatrix(s.x, s.y, s.z);
    }

    template <typename T>
    Matrix<T, 3, 3> Matrix<T, 3, 3>::MakeRotationMatrix(
        const Vector<T, 3>& axis, T rad) {
        return Matrix(
            1 + (1 - std::cos(rad))*(axis.x*axis.x - 1),
            -axis.z*std::sin(rad) + (1 - std::cos(rad))*axis.x*axis.y,
            axis.y*std::sin(rad) + (1 - std::cos(rad))*axis.x*axis.z,

            axis.z*std::sin(rad) + (1 - std::cos(rad))*axis.x*axis.y,
            1 + (1 - std::cos(rad))*(axis.y*axis.y - 1),
            -axis.x*std::sin(rad) + (1 - std::cos(rad))*axis.y*axis.z,

            -axis.y*std::sin(rad) + (1 - std::cos(rad))*axis.x*axis.z,
            axis.x*std::sin(rad) + (1 - std::cos(rad))*axis.y*axis.z,
            1 + (1 - std::cos(rad))*(axis.z*axis.z - 1));
    }




    template<typename T>
    Matrix3x3<T> operator-(const Matrix3x3<T>& a)
    {
        return a.Mul(-1);
    }

    template<typename T>
    Matrix3x3<T> operator+(const Matrix3x3<T>& a, const Matrix3x3<T>& b)
    {
        return a.Add(b);
    }

    template<typename T>
    Matrix3x3<T> operator+(const Matrix3x3<T>& a, T b)
    {
        return a.Add(b);
    }

    template<typename T>
    Matrix3x3<T> operator+(T a, Matrix3x3<T>& b)
    {
        b.RAdd(a);
    }

    template<typename T>
    Matrix3x3<T> operator-(const Matrix3x3<T>& a, const Matrix3x3<T>& b)
    {
        return a.Sub(b);
    }

    template<typename T>
    Matrix3x3<T> operator-(const Matrix3x3<T>&a, T b)
    {
        return a.Sub(b);
    }

    template<typename T>
    Matrix3x3<T> operator-(T a, const Matrix3x3<T>& b)
    {
        b.RSub(a);
    }

    template<typename T>
    Matrix3x3<T> operator*(const Matrix3x3<T>& a, T b)
    {
        return a.Mul(b);
    }

    template<typename T>
    Matrix3x3<T> operator*(T a, const Matrix3x3<T>& b)
    {
        return b.RMul(a);
    }

    template<typename T>
    Vector<T,3> operator*(const Matrix3x3<T>& a, const Vector3<T>& b)
    {
        return a.Mul(b);
    }

    template<typename T>
    Matrix3x3<T> operator*(const Matrix3x3<T>& a, const Matrix3x3<T>& b)
    {
        return a.Mul(b);
    }

    template <typename T>
    Matrix<T, 3, 3> operator/(const Matrix<T, 3, 3>& a, T b) {
        return a.Div(b);
    }

    template <typename T>
    Matrix<T, 3, 3> operator/(T a, const Matrix<T, 3, 3>& b) {
        return b.RDiv(a);
    }
}