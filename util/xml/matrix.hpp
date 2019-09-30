#ifndef __MATRIX_H__
#define __MATRIX_H__

#pragma once

#include <vector>

#ifndef WINDOWS
#undef Success
#endif

#ifdef USE_EIGEN
#include <Eigen/LU>
#include <Eigen/Dense>
#endif

UISOFT_BEGIN_NAMESPACE(framework)

template <typename T>//, int R = 0, int C = 0>
class matrix
{
    public:
        typedef T value_type;

		typedef std::vector<std::vector<T>> value_t_matrix;

    protected:
        virtual const value_type*&  data() const = 0;
        virtual value_type*&        data()       = 0;

    public:
        static bool copy_vector_to_vector(const std::vector<T>& A, std::vector<T>& V)
        {	 
	        for (int i=0; i< (int)std::min(A.size(),V.size()); i++)
	        {
		        V[i] = A[i];
	        }

	        return true;
        }

        /*Initialize a std::vector<std::vector<T>>*/ 
        static bool init_matrix(int m, int n, std::vector<std::vector<T>> &VM)
        {
	        VM.resize(m);
	        for(int i=0; i< m; i++)
	        {
		        VM[i].resize(n, (T)0.0);		 
	        }

	        return true;
        }

        /** Copy a double matrix to another, i.e., C = A */
        static bool copy_matrix(const std::vector< std::vector< T>>& A, std::vector< std::vector< T>>& C)
        {
	        size_t i, j;

	        if (A.empty() || C.empty())
	        {
		        return false;
	        }	        

            size_t m = A.size();
	        size_t n = A[0].size();

            if ((m != C.size()) || (n != C[0].size()))
            {
                return false;
            }

	        for (i = 0; i < m; i++)
	        {
		        for (j = 0; j < n; j++)
		        {
			        C[i][j] = A[i][j];
		        }
	        }

	        return true;
        }

        static bool sum_vector(const std::vector<T>& a, const std::vector<T>& b, std::vector<T>& V)
        {
            if (a.empty() || b.empty())
	        {
		        return false;
	        }
	        int m = a.size();
	        V.resize(m);
	        for(int i=0; i< m; i++)
	        {
		        V[i] = a[i] + b[i];		 
	        }

	        return true;
        }

        /** Add vector b to vector a, i.e., a += b */
        static bool add_to_vector(const std::vector<T>& b, std::vector<T>& a)
        {
	        if (a.size() != b.size())
            {
                return false;
            }

	        for (int i = 0; i < (int)a.size(); i++)
	        {
		        a[i] += b[i];
	        }

	        return true;
        }
            
        static bool subtract_vector(const std::vector<T>& a, const std::vector<T>& b, std::vector<T>& c)
        {
	        if (a.empty() || b.empty())
	        {
		        return false;
	        }

	        size_t m = a.size();
	        c.resize(m);
	        for(size_t i=0; i< m; i++)
	        {
		        c[i] = a[i] - b[i];		 
	        }

	        return true;
        }
        
        /** Subtract double vector b from a and save the answer to a, i.e., a -= b; */
        static bool subtract_from_vector(const std::vector<T>& b, std::vector<T>& a)
        {
            if (a.size() != b.size())
            {
                return false;
            }

	        for (int i = 0; i < (int)b.size(); i++)
	        {
		        a[i] -= b[i];
	        }

	        return true;
        }
                
        static bool matrix_transpose(const std::vector< std::vector< T>>& A, std::vector< std::vector< T>>& C)
        {
	        if (A.empty())
	        {
		        return false;
	        }	    

	        size_t m = A.size();
	        size_t n = A[0].size();

	        for (size_t i = 0; i < m; i++)
	        {
		        for (size_t j = 0; j < n; j++)
		        {
			        C[j][i] = A[i][j];
		        }
	        }

	        return true;
        }
            
        static bool matrix_sum(const std::vector< std::vector< T>>& A, const std::vector< std::vector< T>>& B, std::vector< std::vector< T>>& C)
        {
	        if (A.empty() || B.empty())
	        {
		        return false;
	        }

	        int m = A. size();
	        int n = A[0].size();

	        for (int i = 0; i < m; i++)
	        {
		        for (int j = 0; j < n; j++)
		        {
			        C[i][j] = A[i][j] + B[i][j];
		        }
	        }
	        return true;
        }

        /** Find the difference of two m-by-n matrices, i.e., C = A - B */        
        static bool  matrix_difference(const std::vector< std::vector< T>>& A, const std::vector< std::vector< T>>& B, std::vector< std::vector< T>>& C)
        {

	        if (A.empty() || B.empty())
	        {
		        return false;;
	        }

	        int m = A. size();
	        int n = A[0].size();

	        for (int i = 0; i < m; i++)
	        {
		        for (int j = 0; j < n; j++)
		        {
			        (*C)[i][j] = A[i][j] - B[i][j];
		        }
	        }

	        return true;
        }

        /** Find the product of two std::vector<std::vector>>, i.e., C = A*B */        
        static bool matrix_matrix_product(const std::vector<std::vector<T>>& A, const std::vector<std::vector<T>>& B, std::vector<std::vector<T>>& C)   
        {
	        if (A.empty() || B.empty() )
	        {
		        return false;
	        }  

	        int m = A.size();
	        int l = B[0].size();
	        int n = B.size();// = A[0].size()

	        init_matrix(m, l, C);
	
	        for (int i = 0; i < m; i++)
	        {
		        for (int j = 0; j < l; j++)
		        {
			        C[i][j] = 0.0;
			        for (int k = 0; k < n; k++)
			        {
				        C[i][j] += A[i][k] * B[k][j];
			        }
		        }

	        }
	        return true;
        }

        /** Find the product of a matrix and transpose of another, i.e., C = A*B' */        
        static bool matrix_matrix_transpose_product(const std::vector<std::vector<T>>& A, const std::vector<std::vector<T>>& B, std::vector<std::vector<T>>& C)
        {
	        size_t i, j, k;
            size_t m, n, l;

            m = A.size();
            n = std::min(A[0].size(), B.size());
            l = B[0].size();

	        for (i = 0; i < m; i++)
	        {
		        for (j = 0; j < l; j++)
		        {
			        C[i][j] = 0.0;
			        for (k = 0; k < n; k++)
			        {
				        C[i][j] += A[i][k] * B[j][k];
			        }
		        }

	        }

	        return true;
        }

        /** Find the product of a std::vector<std::vector >> and a std::vector<>, i.e., y = A*x */        
        static bool matrix_vector_product(const std::vector<std::vector<T>>& A, const std::vector<T>& x, std::vector<T>& y)
        {
	        if (A.empty() || x.empty() )
	        {
		        return false;
	        }	    

	        size_t m = A.size();
	        size_t n = x.size();

	        y.resize(n);
	        for (size_t i = 0; i < m; i++)
	        {
		        for (size_t j = 0; j < n; j++)
		        {
			        y[i] += A[i][j] * x[j];
		        }
	        }

	        return true;
        }

        /** Find the weighted std::vector inner product, i.e, c = x'*A*x */        
        static T weighted_vector_inner_product(const std::vector<T>& x, const std::vector<std::vector<T>>& A)
        {
	        T r = 0.0, c;

	        int n = x.size();
	        if (x.empty()|| A.empty())
	        {
		        return r;
	        }

	        r = 0;
	        for (int i = 0; i < n; i++)
	        {
		        c = 0;
		        for (int j = 0; j < n; j++)
		        {
			        c += x[j] * A[j][i];
		        }
		        r += c * x[i];
	        }

	        return r;
        }

        /** Find weighted matrix outer product, i.e., C = A*B*A’ */        
        static bool weighted_matrix_outer_product(const std::vector<std::vector<T>>& A, const std::vector<std::vector<T>>& B, std::vector<std::vector<T>>& C)
        {
	        if (A.empty() || B.empty())
	        {
		        return false;
	        }	    

	        size_t m = A.size();
	        size_t n = A[0].size();

	        for (size_t i = 0; i < m; i++)
	        {
		        for (size_t j = 0; j < m; j++)
		        {
			        C[i][j] = 0.0;
			        for (size_t k = 0; k < n; k++)
			        {
				        T c = 0.0;
				        for (size_t l = 0; l < n; l++)
				        {
					        c += A[i][l] * B[l][k];
				        }
				        C[i][j] += c * A[j][k];
			        }
		        }
	        }

	        return true;
        }

        /** Find the inverse of symmetric positive semi-definite matrix, i.e., C = inv(A) */
        static bool symmetric_positive_semidefinite_matrix_inverse(const std::vector<std::vector<T>>& A, std::vector< std::vector<T>> &C)
        {
	        /*what's the use of pin and Bin*/
	        int i,j,k;
	        std::vector<std::vector<T>> B;
	        std::vector<T> p;
	        T sum;
	        bool err = false;

	        if (A.empty())
	        {
		        return false;
	        }
	    
	        int n = A.size();
	        p.resize(n);
	        
	        init_matrix(n, n, &B);	    
	        

	        for (i = 0; i < n; i++)
	        {
		        for (j = 0; j < n; j++)
		        {
			        B[i][j] = A[i][j];
		        }
	        }

	        for (i = 0; i < n; i++)
	        {
		        for (j = i; j < n; j++)
		        {
			        for (sum = B[i][j], k = i - 1; k >= 0; k--)
			        {
				        sum -= B[i][k] * B[j][k];
			        }
			        if (i == j)
			        {
				        if (sum <= 0.0)
				        {
					        err = true;
				        }
				        else
					        p[i] = sqrt(sum);
			        }
			        else
			        {
				        ASSERT_DIV("p[i]", p[i]);
				        B[j][i] = sum / p[i];
			        }
		        }
	        }

	        for (i = 0; i < n; i++)
	        {
		        B[i][i] = p[i];
		        for (j = i + 1; j < n; j++)
			        B[i][j] = 0;
	        }

	        for (i = 0; i < n; i++)
	        {
		        ASSERT_DIV("p[i]", p[i]);
		        B[i][i] = 1.0 / p[i];
		        for (j = i + 1; j < n; j++)
		        {
			        sum = 0.0;
			        for (k = i; k < j; k++)
				        sum -= B[j][k] * B[k][i];
			        ASSERT_DIV("p[j]", p[j]);
			        B[j][i] = sum / p[j];
		        }
	        }

	        for (i = 0; i < n; i++)
	        {
		        for (j = 0; j < n; j++)
		        {
			        C[i][j] = 0.0;
			        for (k = 0; k < n; k++)
			        {
				        C[i][j] += B[k][i] * B[k][j];
			        }
		        }
	        }

	        return true;
        }
        
#ifdef USE_EIGEN
		static bool matrix_inverse(const std::vector<std::vector<T>>& A, std::vector< std::vector<T>> &C)
		{
			if (A.empty() || C.empty())
			{
				return false;
			}

			int m = A.size();
			int n = A[0].size();
			Eigen::MatrixXd Ao(m, n);
			Eigen::MatrixXd inv_A(m, n);

			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					Ao(i, j) = (double)A[i][j];
				}
			}
			inv_A = Ao.inverse();

			init_matrix(m, n, C);
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					C[i][j] = inv_A(i,j);
				}
			}

			return true;
		}
#endif

        /** Find the normalized std::vector inner product, i.e., c = x’*inv(A)*x */
        static T normalized_vector_inner_product(const std::vector<T>& x, const std::vector<std::vector<T>>& A, bool is_inverse)
        {
	        std::vector<std::vector<T>> B;
	        T c = 0.0;

	        if (!x.empty() && !A.empty())
	        {
		        int m = A.size();
		        if (is_inverse)
		        {
			        B = A;
		        }
		        else
		        {
			        init_matrix(m, m, &B);
			   
			        symmetric_positive_semidefinite_matrix_inverse (A, &B);
		        }

		        c = weighted_vector_inner_product(x, B);
	        }
	        return c;

        }

        /** Find the inner product of a vector, i.e., answer = a'*a */
        static T vector_inner_product(const std::vector<T>& a)
        {
	        int i;
	        T c = 0.0;

	        if (!a.empty())
	        {
		        int m = a.size();
		        for (i = 0; i < m; i++)
		        {
			        c += SQR(a[i]);
		        }
	        }
	        return c;
        }

        /** Find the outer product of a std::vector, i.e., C = a*a' */
        static bool vector_outer_product(const std::vector<T>& a, std::vector<std::vector<T>>& C)
        {
	        int i, j;

	        if (a.empty())
	        {
		        return false;
	        }
	        
	        int m = a.size();
	        for (i = 0; i < m; i++)
	        {
		        for (j = 0; j < m; j++)
		        {
			        C[i][j] = a[i] * a[j];
		        }
	        }

	        return true;
        }

        /** Find the outer product of two vectors, i.e., C = a*b' */
        static bool vector_vector_outer_product(const std::vector<T>& a, const std::vector<T>& b, std::vector<std::vector<T>>& C)
        {
	        int i, j;
	        if (a.empty() || b.empty())
	        {
		        return false;
	        }	    

	        int m = a.size();
	        int n = b.size();
	        for (i = 0; i < m; i++)
	        {
		        for (j = 0; j < n; j++)
		        {
			        C[i][j] = a[i] * b[j];
		        }
	        }

	        return true;
        }

        /** Find the inner product of two vectors, i.e., c = a'*b */
        static T vector_vector_inner_product(const std::vector<T>& a, const std::vector<T>& b)
        {
	        int i;
	        T c = 0.0;
	        int m = a.size();

	        if (!a.empty() && !b.empty())
	        {
		        for (i = 0; i < m; i++)
		        {
			        c += a[i] * b[i];
		        }
	        }
	        return c;
        }

        /** Find the product of matrix and a scalar, i.e., C = s*A */
        static bool scalar_matrix_product(T s, const std::vector<std::vector<T>>& A, std::vector<std::vector<T>>& C)
        {
	        int i, j;

	        if (A.empty())
	        {
		        return false;
	        }
	    
	        int m = A.size();
	        int n = A[0].size();

	        for (i = 0; i < m; i++)
	        {
		        for (j = 0; j < n; j++)
		        {
			        C[i][j] = A[i][j] * s;
		        }
	        }

	        return true;
        }

        /** Scale a vector with given value, i.e., a[:] *= s */
        static bool scale_vector(T s, std::vector<T>& a)
        {
	        int i;

	        if (a.empty() )
	        {
		        return false;
	        }
	    
	        int m = a.size();
	        for (i = 0; i < m; i++)
	        {
		        a[i] *= s;
	        }

	        return true;
        }

        /** Find the product of vector and a scalar, i.e., c = s*a */
        static bool scalar_vector_product(T s, const std::vector<T>& a, std::vector<T>& c)
        {
	        int i;

	        if (a.empty())
	        {
		        return false;
	        }
	    
	        int m = a.size();
	        for (i = 0; i < m; i++)
	        {
		        c[i] = a[i] * s;
	        }

	        return true;
        }

        /** Scale a double matrix with given value, i.e., a[:][:] *= s */
        static bool scale_matrix(T s, std::vector<std::vector<T>>& C)
        {
	        size_t i, j;

	        if (C.empty())
	        {
		        return false;
	        }

            size_t m = C.size();
	        size_t n = C[0].size();
	        for (i = 0; i < m; i++)
	        {
		        for (j = 0; j < n; j++)
		        {
			        C[i][j] *= s;
		        }
	        }

	        return true;
        }
};

UISOFT_END_NAMESPACE

#endif // __MATRIX_H__
