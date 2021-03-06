// Copyright (C) 2015  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.


#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "../dnn.h"

#include "tester.h"


namespace  
{

    using namespace test;
    using namespace dlib;
    using namespace std;

    logger dlog("test.dnn");

// ----------------------------------------------------------------------------------------

    template <typename T>
    float compare_gradients (
        const tensor& t,
        T grad
    )
    {
        float max_error = 0;
        auto p = t.host();
        for (size_t i = 0; i < t.size(); ++i)
        {
            max_error = std::max(max_error, std::abs(p[i]-grad(i)));
        }
        return max_error;
    }

// ----------------------------------------------------------------------------------------

    void test_tanh()
    {
        using namespace dlib::tt;
        print_spinner();
        resizable_tensor src(5,5), dest(5,5), gradient_input(5,5);
        src = matrix_cast<float>(gaussian_randm(5,5, 0));
        dest = matrix_cast<float>(gaussian_randm(5,5, 1));
        gradient_input = matrix_cast<float>(gaussian_randm(5,5, 2));



        auto grad_src = [&](long idx) {
            auto f = [&](float eps) {
                const float old = src.host()[idx];
                src.host()[idx] += eps;
                tanh(dest, src);
                float result = dot(gradient_input, dest);
                src.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };

        resizable_tensor src_grad;
        src_grad.copy_size(src);
        src_grad = 0;

        tanh(dest, src);
        tanh_gradient(src_grad, dest, gradient_input);

        auto grad_error = compare_gradients(src_grad, grad_src);
        dlog << LINFO << "src error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);
    }

    void test_sigmoid()
    {
        using namespace dlib::tt;
        print_spinner();
        resizable_tensor src(5,5), dest(5,5), gradient_input(5,5);
        src = matrix_cast<float>(gaussian_randm(5,5, 0));
        dest = matrix_cast<float>(gaussian_randm(5,5, 1));
        gradient_input = matrix_cast<float>(gaussian_randm(5,5, 2));



        auto grad_src = [&](long idx) {
            auto f = [&](float eps) {
                const float old = src.host()[idx];
                src.host()[idx] += eps;
                sigmoid(dest, src);
                float result = dot(gradient_input, dest);
                src.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };

        resizable_tensor src_grad;
        src_grad.copy_size(src);
        src_grad = 0;

        sigmoid(dest, src);
        sigmoid_gradient(src_grad, dest, gradient_input);

        auto grad_error = compare_gradients(src_grad, grad_src);
        dlog << LINFO << "src error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);
    }

    void test_softmax()
    {
        using namespace dlib::tt;
        print_spinner();
        const long nr = 3;
        const long nc = 3;
        resizable_tensor src(5,5,nr,nr), dest(5,5,nr,nc), gradient_input(5,5,nr,nc);
        src = matrix_cast<float>(gaussian_randm(5,5*nr*nc, 0));
        dest = matrix_cast<float>(gaussian_randm(5,5*nr*nc, 1));
        gradient_input = matrix_cast<float>(gaussian_randm(5,5*nr*nc, 2));



        auto grad_src = [&](long idx) {
            auto f = [&](float eps) {
                const float old = src.host()[idx];
                src.host()[idx] += eps;
                tt::softmax(dest, src);
                float result = dot(gradient_input, dest);
                src.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };

        resizable_tensor src_grad;
        src_grad.copy_size(src);
        src_grad = 0;

        tt::softmax(dest, src);
        softmax_gradient(src_grad, dest, gradient_input);

        auto grad_error = compare_gradients(src_grad, grad_src);
        dlog << LINFO << "src error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);
    }

    void test_batch_normalize()
    {
        using namespace dlib::tt;
        print_spinner();
        resizable_tensor src(5,5), gamma(1,5), beta(1,5), dest, dest2, means, vars, gradient_input(5,5);
        src = matrix_cast<float>(gaussian_randm(5,5, 0));
        gamma = matrix_cast<float>(gaussian_randm(1,5, 1));
        beta = matrix_cast<float>(gaussian_randm(1,5, 2));
        gradient_input = matrix_cast<float>(gaussian_randm(5,5, 3));

        gamma = 1;
        beta = 0;

        resizable_tensor running_means;
        resizable_tensor running_invstds;
        batch_normalize(dest, means, vars, 1, running_means, running_invstds, src, gamma, beta);
        batch_normalize_inference(dest2, src, gamma, beta, running_means, running_invstds);
        DLIB_TEST(max(abs(mat(dest2)-mat(dest))) < 1e-5);


        auto grad_src = [&](long idx) {
            auto f = [&](float eps) {
                const float old = src.host()[idx];
                src.host()[idx] += eps;
                batch_normalize(dest, means, vars, 1, running_means, running_invstds, src, gamma, beta);
                float result = dot(gradient_input, dest);
                src.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };
        auto grad_gamma = [&](long idx) {
            auto f = [&](float eps) {
                const float old = gamma.host()[idx];
                gamma.host()[idx] += eps;
                batch_normalize(dest, means, vars, 1, running_means, running_invstds, src, gamma, beta);
                float result = dot(gradient_input, dest);
                gamma.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };
        auto grad_beta = [&](long idx) {
            auto f = [&](float eps) {
                const float old = beta.host()[idx];
                beta.host()[idx] += eps;
                batch_normalize(dest, means, vars, 1, running_means, running_invstds, src, gamma, beta);
                float result = dot(gradient_input, dest);
                beta.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };

        resizable_tensor src_grad, gamma_grad, beta_grad;
        src_grad.copy_size(src);
        gamma_grad.copy_size(gamma);
        beta_grad.copy_size(beta);
        src_grad = 0;
        gamma_grad = 8;
        beta_grad = 8;

        batch_normalize_gradient(gradient_input, means, vars, src, gamma, src_grad, gamma_grad, beta_grad);

        auto grad_error = compare_gradients(src_grad, grad_src);
        dlog << LINFO << "src error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);

        grad_error = compare_gradients(gamma_grad, grad_gamma);
        dlog << LINFO << "gamma error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);

        grad_error = compare_gradients(beta_grad, grad_beta);
        dlog << LINFO << "beta error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);
    }

    void test_batch_normalize_conv()
    {
        using namespace dlib::tt;
        print_spinner();
        resizable_tensor src(5,5,4,4), gamma(1,5), beta(1,5), dest, dest2, means, vars, gradient_input(5,5,4,4);
        src = matrix_cast<float>(gaussian_randm(5,5*4*4, 0));
        gamma = matrix_cast<float>(gaussian_randm(1,5, 1));
        beta = matrix_cast<float>(gaussian_randm(1,5, 2));
        gradient_input = matrix_cast<float>(gaussian_randm(5,5*4*4, 3));

        gamma = 1;
        beta = 0;

        resizable_tensor running_means;
        resizable_tensor running_invstds;
        batch_normalize_conv(dest, means, vars, 1, running_means, running_invstds, src, gamma, beta);
        batch_normalize_conv_inference(dest2, src, gamma, beta, running_means, running_invstds);
        DLIB_TEST(max(abs(mat(dest2)-mat(dest))) < 1e-5);


        auto grad_src = [&](long idx) {
            auto f = [&](float eps) {
                const float old = src.host()[idx];
                src.host()[idx] += eps;
                batch_normalize_conv(dest, means, vars, 1, running_means, running_invstds, src, gamma, beta);
                float result = dot(gradient_input, dest);
                src.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };
        auto grad_gamma = [&](long idx) {
            auto f = [&](float eps) {
                const float old = gamma.host()[idx];
                gamma.host()[idx] += eps;
                batch_normalize_conv(dest, means, vars, 1, running_means, running_invstds, src, gamma, beta);
                float result = dot(gradient_input, dest);
                gamma.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };
        auto grad_beta = [&](long idx) {
            auto f = [&](float eps) {
                const float old = beta.host()[idx];
                beta.host()[idx] += eps;
                batch_normalize_conv(dest, means, vars, 1, running_means, running_invstds, src, gamma, beta);
                float result = dot(gradient_input, dest);
                beta.host()[idx] = old;
                return result;
            };
            const float eps = 0.01;
            return (f(+eps)-f(-eps))/(2*eps);
        };


        resizable_tensor src_grad, gamma_grad, beta_grad;
        src_grad.copy_size(src);
        gamma_grad.copy_size(gamma);
        beta_grad.copy_size(beta);
        src_grad = 0;
        gamma_grad = 9;
        beta_grad = 9;

        batch_normalize_conv_gradient(gradient_input, means, vars, src, gamma, src_grad, gamma_grad, beta_grad);


        auto grad_error = compare_gradients(src_grad, grad_src);
        dlog << LINFO << "src error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);

        grad_error = compare_gradients(gamma_grad, grad_gamma);
        dlog << LINFO << "gamma error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);

        grad_error = compare_gradients(beta_grad, grad_beta);
        dlog << LINFO << "beta error: " << grad_error;
        DLIB_TEST(grad_error < 0.001);

    }

// ----------------------------------------------------------------------------------------

    void test_basic_tensor_ops()
    {
        using namespace dlib::tt;
        print_spinner();
        resizable_tensor dest, src(3,4), A(1,4), B(1,4);
        src = 2;
        dest.copy_size(src);
        affine_transform(dest, src, 2, 3);
        dlog << LINFO << mat(dest);
        matrix<float> truth1(3,4), truth2(3,4);

        truth1 = 7;
        truth2 = 7, 10,  7,  7,
        7, 10,  7,  7,
        7, 10,  7,  7;
        DLIB_TEST(max(abs(truth1-mat(dest))) < 1e-5);

        A = 2;
        B = 3;
        A.host()[1] = 3;
        B.host()[1] = 4;
        dest = 0;
        affine_transform(dest, src, A, B);
        dlog << LINFO << mat(dest);
        DLIB_TEST(max(abs(truth2-mat(dest))) < 1e-5);

        A.set_size(3,4);
        B.set_size(3,4);
        A = matrix_cast<float>(gaussian_randm(3,4, 1));
        B = matrix_cast<float>(gaussian_randm(3,4, 2));
        affine_transform(dest, src, A, B);
        dlog << LINFO << mat(dest);
        matrix<float> truth3 = pointwise_multiply(mat(src), mat(A)) + mat(B);
        DLIB_TEST(max(abs(truth3-mat(dest))) < 1e-5);

        matrix<float> truth4 = pointwise_multiply(mat(A), mat(B));
        tt::multiply(A, A, B);
        DLIB_TEST(max(abs(truth4-mat(A))) < 1e-5);

        matrix<float> truth5 = mat(B) > 0.1;
        dlog << LINFO << truth5;
        threshold(B, 0.1);
        DLIB_TEST(max(abs(truth5-mat(B))) < 1e-5);

        int cnt = 0;
        for(auto& x : A)
            x = cnt++;

        truth1.set_size(2,2);
        truth2.set_size(2,2);
        truth3.set_size(2,2);
        truth1 = 0,1,2,3;
        truth2 = 4,5,6,7;
        truth3 = 8,9,10,11;

        alias_tensor at(2,2);
        auto A0 = at(A,0);
        auto A4 = at(A,4);
        auto A8 = at(A,8);
        DLIB_TEST(mat(A0) == truth1);
        DLIB_TEST(mat(at(A,4)) == truth2);
        DLIB_TEST(mat(A8) == truth3);

        A4 += uniform_matrix<float>(2,2,2);
        truth2 += 2;
        DLIB_TEST(mat(A4) == truth2);
        truth1 = trans(reshape_to_column_vector(truth1));
        truth2 = trans(reshape_to_column_vector(truth2));
        truth3 = trans(reshape_to_column_vector(truth3));

        DLIB_TEST(mat(A) == join_cols(truth1,join_cols(truth2,truth3)));

        affine_transform(A,A,1,2);
        truth1 += 2;
        truth2 += 2;
        truth3 += 2;
        DLIB_TEST(mat(at(A,4)) == reshape(truth2,2,2));
        DLIB_TEST(mat(A) == join_cols(truth1,join_cols(truth2,truth3)));

        {
            resizable_tensor dest(3,4);
            resizable_tensor A, B;
            A = dest;
            B = dest;

            tensor_rand rnd;
            rnd.fill_uniform(dest);
            rnd.fill_uniform(A);
            rnd.fill_uniform(B);

            dest.set_size(1,4);

            tt::multiply(dest, A, B);
            DLIB_TEST(max(abs(mat(dest)-sum_rows(pointwise_multiply(mat(A),mat(B))))) < 1e-6); 

            A.set_size(1,4);
            rnd.fill_uniform(A);
            matrix<float> AA = join_cols(mat(A),mat(A)); AA = join_cols(mat(A),AA);

            tt::multiply(dest, A, B);
            DLIB_TEST(max(abs(mat(dest)-sum_rows(pointwise_multiply(AA,mat(B))))) < 1e-6); 

            tt::multiply(dest, B, A);
            DLIB_TEST(max(abs(mat(dest)-sum_rows(pointwise_multiply(AA,mat(B))))) < 1e-6); 

            dest.set_size(3,4);
            tt::multiply(dest, B, A);
            DLIB_TEST(max(abs(mat(dest)-pointwise_multiply(AA,mat(B)))) < 1e-6); 

            tt::multiply(dest, A, B);
            DLIB_TEST(max(abs(mat(dest)-pointwise_multiply(AA,mat(B)))) < 1e-6); 
        }

        {
            resizable_tensor A, B;
            A.set_size(2,3,4,5);
            B.set_size(2,3,4,5);

            tensor_rand rnd;
            rnd.fill_uniform(A);
            rnd.fill_uniform(B);

            matrix<float> truth;

            truth = 2*mat(A) + 3*mat(B);
            tt::add(2, A, 3, B);
            DLIB_TEST(max(abs(mat(A)-truth )) < 1e-6);


            rnd.fill_uniform(A);
            rnd.fill_uniform(B);
            truth = 0*mat(A) + 3*mat(B);
            tt::add(0, A, 3, B);
            DLIB_TEST(max(abs(mat(A)-truth )) < 1e-6);

            rnd.fill_uniform(A);
            rnd.fill_uniform(B);
            truth = 1*mat(A) + 0*mat(B);
            tt::add(1, A, 0, B);
            DLIB_TEST(max(abs(mat(A)-truth )) < 1e-6);


            rnd.fill_uniform(A);
            rnd.fill_uniform(B);
            truth = 0*mat(A) + 0*mat(B);
            tt::add(0, A, 0, B);
            DLIB_TEST(max(abs(mat(A)-truth )) < 1e-6);


            B.set_size(1,3,4,5);
            rnd.fill_uniform(A);
            rnd.fill_uniform(B);
            truth = 2*mat(A) + 3*join_cols(mat(B), mat(B));
            tt::add(2, A, 3, B);
            DLIB_TEST(max(abs(mat(A)-truth )) < 1e-6);
            DLIB_TEST(A.num_samples()==2);

            B.set_size(1,1,4,5);
            rnd.fill_uniform(A);
            rnd.fill_uniform(B);
            matrix<float> temp = join_rows(mat(B), join_rows(mat(B),mat(B)));
            truth = 2*mat(A) + 3*join_cols(temp,temp);
            tt::add(2, A, 3, B);
            DLIB_TEST_MSG(max(abs(mat(A)-truth )) < 1e-6, max(abs(mat(A)-truth )));

            B.set_size(1,3,1,1);
            rnd.fill_uniform(A);
            rnd.fill_uniform(B);
            resizable_tensor AA(A), BB(B);
            tt::add(2, A, 3, B);
            cpu::add(2, AA, 3, BB);
            DLIB_TEST_MSG(max(abs(mat(A)-mat(AA) )) < 1e-6, max(abs(mat(A)-mat(AA) )));
        }
    }

// ----------------------------------------------------------------------------------------

#ifdef DLIB_USE_CUDA

    void test_conv()
    {
        cuda::tensor_conv conv1;
        cpu::tensor_conv conv2;

        dlib::rand prnd;
        for (int iter = 0; iter < 400; ++iter)
        {
            print_spinner();

            resizable_tensor data(prnd.get_random_32bit_number()%5+1,
                prnd.get_random_32bit_number()%5+1,
                prnd.get_random_32bit_number()%30+1,
                prnd.get_random_32bit_number()%30+1
            );
            resizable_tensor filters(
                prnd.get_random_32bit_number()%5+1,
                data.k(),
                prnd.get_random_32bit_number()%6+1,
                prnd.get_random_32bit_number()%6+1 
            );

            tt::tensor_rand rnd;
            rnd.fill_uniform(data);
            rnd.fill_uniform(filters);


            resizable_tensor output1, output2;


            const int stride_y = prnd.get_random_32bit_number()%5+1;
            const int stride_x = prnd.get_random_32bit_number()%5+1;
            conv1(output1, data, filters, stride_y,stride_x);

            conv2(output2, data, filters, stride_y,stride_x);

            dlog << LINFO << "forward error: "<< max(abs(mat(output1)-mat(output2)));
            DLIB_TEST(max(abs(mat(output1)-mat(output2))) < 1e-3);



            resizable_tensor gi, data_gradient1, data_gradient2;
            gi.copy_size(output1);
            rnd.fill_uniform(gi);

            data_gradient1.copy_size(data);
            data_gradient2.copy_size(data);
            data_gradient1 = 1;
            data_gradient2 = 1;

            conv1.get_gradient_for_data(gi, filters, data_gradient1);
            conv2.get_gradient_for_data(gi, filters, data_gradient2);

            dlog << LINFO << "data gradient error: "<< max(abs(mat(data_gradient1)-mat(data_gradient2)));
            DLIB_TEST(max(abs(mat(data_gradient1)-mat(data_gradient2))) < 1e-3);


            resizable_tensor filter_gradient1, filter_gradient2;
            gi.copy_size(output1);
            rnd.fill_uniform(gi);

            filter_gradient1.copy_size(filters);
            filter_gradient2.copy_size(filters);
            filter_gradient1 = 1;
            filter_gradient2 = 1;

            conv1.get_gradient_for_filters(gi, data, filter_gradient1);
            conv2.get_gradient_for_filters(gi, data, filter_gradient2);

            dlog << LINFO << "filter gradient error: "<< max(abs(mat(filter_gradient1)-mat(filter_gradient2)));
            DLIB_TEST(max(abs(mat(filter_gradient1)-mat(filter_gradient2))) < 1e-3);
        }
    }

    void compare_adam()
    {
        float t = 2;
        tt::tensor_rand rnd;
        resizable_tensor s, m, v, params, params_grad;
        s.set_size(89,90,60,73);
        m.copy_size(s);
        v.copy_size(s);
        params.copy_size(s);
        params_grad.copy_size(s);

        rnd.fill_uniform(s);
        rnd.fill_uniform(m);
        rnd.fill_uniform(v);
        rnd.fill_uniform(params);
        rnd.fill_uniform(params_grad);

        resizable_tensor mm(m), vv(v);
        cpu::compute_adam_update(s, mm, vv, t, 0.01, 0.001, 0.9, 0.99, params, params_grad);
        matrix<float> s1 = mat(s);
        
        rnd.fill_uniform(s);
        cuda::compute_adam_update(s, m, v, t, 0.01, 0.001, 0.9, 0.99, params, params_grad);
        matrix<float> s2 = mat(s);

        DLIB_TEST_MSG(max(abs(s1-s2)) < 1e-6, max(abs(s1-s2)));
        DLIB_TEST_MSG(max(abs(mat(m)-mat(mm))) < 1e-6, max(abs(mat(m)-mat(mm))));
        DLIB_TEST_MSG(max(abs(mat(v)-mat(vv))) < 1e-6, max(abs(mat(v)-mat(vv))));
    }

    void test_add()
    {
        print_spinner();
        dlib::rand rnd;
        tt::tensor_rand trnd;
        for (int iter = 0; iter < 300; ++iter)
        {
            resizable_tensor dest1(rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1);
            resizable_tensor dest2;
            dest2.copy_size(dest1);
            resizable_tensor src1(rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1);
            resizable_tensor src2(rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1,
                                  rnd.get_random_32bit_number()%4+1);

            trnd.fill_uniform(dest1);
            trnd.fill_uniform(dest2);
            trnd.fill_uniform(src1);
            trnd.fill_uniform(src2);
            cpu::add(dest1, src1, src2);
            cuda::add(dest2, src1, src2);

            DLIB_TEST(max(abs(mat(dest1) - mat(dest2))) < 1e-5);
        }

        // make sure we have a test for the case where all tensors have the same
        // dimensions.
        resizable_tensor dest1(3,4,5,6);
        resizable_tensor dest2;
        resizable_tensor src1;
        resizable_tensor src2;
        dest2.copy_size(dest1);
        src1.copy_size(dest1);
        src2.copy_size(dest1);

        trnd.fill_uniform(dest1);
        trnd.fill_uniform(dest2);
        trnd.fill_uniform(src1);
        trnd.fill_uniform(src2);

        cpu::add(dest1, src1, src2);
        cuda::add(dest2, src1, src2);

        DLIB_TEST(max(abs(mat(dest1) - mat(dest2))) < 1e-5);
    }

    void test_more_ops(const long nr, const long nc)
    {
        using namespace dlib::tt;
        print_spinner();
        // We are going to make sure that the CPU implementation of these things matches
        // the CUDA implementation.

        tensor_rand rnd;

        resizable_tensor dest(nr,nc), src(nr,nc), dest2, src2;
        resizable_tensor srcb(nr,nc), srcc(nr,nc), srcb2, srcc2;


        rnd.fill_uniform(dest);
        rnd.fill_uniform(src);
        dest2 = dest; src2 = src;
        cuda::multiply(dest, dest, src);
        cpu::multiply(dest2, dest2, src2);
        DLIB_TEST(equal(mat(dest),mat(dest2)));


        rnd.fill_uniform(dest);
        rnd.fill_uniform(src);
        dest2 = dest; src2 = src;
        cuda::affine_transform(dest, src, 2, 3);
        cpu::affine_transform(dest2, src2, 2, 3);
        DLIB_TEST(equal(mat(dest),mat(dest2)));

        rnd.fill_uniform(dest);
        rnd.fill_uniform(src);
        rnd.fill_uniform(srcb);
        dest2 = dest; src2 = src; srcb2 = srcb;
        cuda::affine_transform(dest, src, srcb, 2, 3, 4);
        cpu::affine_transform(dest2, src2, srcb2, 2, 3, 4);
        DLIB_TEST(equal(mat(dest),mat(dest2)));

        rnd.fill_uniform(dest);
        rnd.fill_uniform(src);
        rnd.fill_uniform(srcb);
        rnd.fill_uniform(srcc);
        dest2 = dest; src2 = src; srcb2 = srcb; srcc2 = srcc;
        cuda::affine_transform(dest, src, srcb, srcc, 2, 3, 4, 5);
        cpu::affine_transform(dest2, src2, srcb2, srcc2, 2, 3, 4, 5);
        DLIB_TEST(equal(mat(dest),mat(dest2)));


        rnd.fill_uniform(dest);
        rnd.fill_uniform(src);
        rnd.fill_uniform(srcb);
        rnd.fill_uniform(srcc);
        dest2 = dest; src2 = src; srcb2 = srcb; srcc2 = srcc;
        cuda::affine_transform(dest, src, srcb, srcc);
        cpu::affine_transform(dest2, src2, srcb2, srcc2);
        DLIB_TEST(equal(mat(dest),mat(dest2)));
        // now exercise code path where the A/B tensors have num_samples()==1
        srcb.set_size(1,nc);
        srcc.set_size(1,nc);
        rnd.fill_uniform(dest);
        rnd.fill_uniform(src);
        rnd.fill_uniform(srcb);
        rnd.fill_uniform(srcc);
        dest2 = dest; src2 = src; srcb2 = srcb; srcc2 = srcc;
        cuda::affine_transform(dest, src, srcb, srcc);
        cpu::affine_transform(dest2, src2, srcb2, srcc2);
        DLIB_TEST(equal(mat(dest),mat(dest2)));


        rnd.fill_uniform(src);
        src2 = src;
        cuda::threshold(src, 0.5);
        cpu::threshold(src2, 0.5);
        DLIB_TEST(equal(mat(src),mat(src2)));

        {
            resizable_tensor dest(3,4);
            resizable_tensor A, B;
            A = dest;
            B = dest;

            tensor_rand rnd;
            rnd.fill_uniform(dest);
            rnd.fill_uniform(A);
            rnd.fill_uniform(B);

            dest.set_size(1,4);

            cuda::multiply(dest, A, B);
            DLIB_TEST_MSG(max(abs(mat(dest)-sum_rows(pointwise_multiply(mat(A),mat(B))))) < 1e-6, max(abs(mat(dest)-sum_rows(pointwise_multiply(mat(A),mat(B)))))); 

            A.set_size(1,4);
            rnd.fill_uniform(A);
            matrix<float> AA = join_cols(mat(A),mat(A)); AA = join_cols(mat(A),AA);

            cuda::multiply(dest, A, B);
            DLIB_TEST(max(abs(mat(dest)-sum_rows(pointwise_multiply(AA,mat(B))))) < 1e-6); 

            cuda::multiply(dest, B, A);
            DLIB_TEST(max(abs(mat(dest)-sum_rows(pointwise_multiply(AA,mat(B))))) < 1e-6); 

            dest.set_size(3,4);
            cuda::multiply(dest, B, A);
            DLIB_TEST(max(abs(mat(dest)-pointwise_multiply(AA,mat(B)))) < 1e-6); 

            cuda::multiply(dest, A, B);
            DLIB_TEST(max(abs(mat(dest)-pointwise_multiply(AA,mat(B)))) < 1e-6); 
        }
    }

// ----------------------------------------------------------------------------------------

    void compare_bn_gpu_and_cpu()
    {
        print_spinner();
        resizable_tensor dest, dest2;
        resizable_tensor means, means2;
        resizable_tensor invstds, invstds2;
        resizable_tensor running_means, running_means2;
        resizable_tensor running_invstds, running_invstds2;
        resizable_tensor src(64,20,100,100);
        resizable_tensor gamma(1,20,100,100);
        resizable_tensor beta(1,20,100,100);
        gamma = 2;
        beta = 3;
        tt::tensor_rand rnd;
        rnd.fill_uniform(src);


        cpu::batch_normalize(dest, means, invstds, 1, running_means, running_invstds, src, gamma, beta);
        cuda::batch_normalize(dest2,means2,invstds2, 1, running_means2, running_invstds2, src, gamma, beta);

        dlog << LINFO << "dest error:    "<< max(abs(mat(dest) -mat(dest2)));
        dlog << LINFO << "means error:   "<< max(abs(mat(means) -mat(means2)));
        dlog << LINFO << "invstds error: "<< max(abs(mat(invstds) -mat(invstds2)));
        dlog << LINFO << "running_means error:   "<< max(abs(mat(running_means) -mat(running_means2)));
        dlog << LINFO << "running_invstds error: "<< max(abs(mat(running_invstds) -mat(running_invstds2)));

        DLIB_TEST(max(abs(mat(dest) -mat(dest2))) < 1e-4);
        DLIB_TEST(max(abs(mat(means) -mat(means2))) < 1e-4);
        DLIB_TEST(max(abs(mat(invstds) -mat(invstds2))) < 1e-4);
        DLIB_TEST(max(abs(mat(running_means) -mat(running_means2))) < 1e-4);
        DLIB_TEST(max(abs(mat(running_invstds) -mat(running_invstds2))) < 1e-4);


        // now check that the gradients match as well
        resizable_tensor gradient_input;
        resizable_tensor src_grad, gamma_grad, beta_grad;
        resizable_tensor src_grad2, gamma_grad2, beta_grad2;
        gradient_input.copy_size(dest);
        src_grad.copy_size(src); src_grad = 0; src_grad2 = src_grad;
        gamma_grad.copy_size(gamma); gamma_grad = 0; gamma_grad2 = gamma_grad;
        beta_grad.copy_size(beta); beta_grad = 0; beta_grad2 = beta_grad;
        rnd.fill_uniform(gradient_input);


        cpu::batch_normalize_gradient(gradient_input, means, invstds, src, gamma, src_grad, gamma_grad, beta_grad);
        cuda::batch_normalize_gradient(gradient_input, means, invstds, src, gamma, src_grad2, gamma_grad2, beta_grad2);

        dlog << LINFO << "src_grad error:   " << max(abs(mat(src_grad)-mat(src_grad2)));
        dlog << LINFO << "gamma_grad error: " << max(abs(mat(gamma_grad)-mat(gamma_grad2)));
        dlog << LINFO << "beta_grad error:  " << max(abs(mat(beta_grad)-mat(beta_grad2)));
        DLIB_TEST(max(abs(mat(src_grad)-mat(src_grad2))) < 1e-4);
        DLIB_TEST(max(abs(mat(gamma_grad)-mat(gamma_grad2))) < 1e-4);
        DLIB_TEST(max(abs(mat(beta_grad)-mat(beta_grad2))) < 1e-4);
    }

    void compare_bn_conv_gpu_and_cpu()
    {
        print_spinner();
        resizable_tensor dest, dest2;
        resizable_tensor means, means2;
        resizable_tensor invstds, invstds2;
        resizable_tensor running_means, running_means2;
        resizable_tensor running_invstds, running_invstds2;
        resizable_tensor src(2,8,10,9);
        resizable_tensor gamma(1,8);
        resizable_tensor beta(1,8);
        gamma = 2;
        beta = 3;
        tt::tensor_rand rnd;
        rnd.fill_uniform(src);

        cpu::batch_normalize_conv(dest,means,invstds,1,running_means,running_invstds, src, gamma, beta);
        cuda::batch_normalize_conv(dest2,means2,invstds2,1,running_means2,running_invstds2, src, gamma, beta);

        dlog << LINFO << "dest error:    "<< max(abs(mat(dest) -mat(dest2)));
        dlog << LINFO << "means error:   "<< max(abs(mat(means) -mat(means2)));
        dlog << LINFO << "invstds error: "<< max(abs(mat(invstds) -mat(invstds2)));
        dlog << LINFO << "running_means error:   "<< max(abs(mat(running_means) -mat(running_means2)));
        dlog << LINFO << "running_invstds error: "<< max(abs(mat(running_invstds) -mat(running_invstds2)));

        DLIB_TEST(max(abs(mat(dest) -mat(dest2))) < 1e-4);
        DLIB_TEST(max(abs(mat(means) -mat(means2))) < 1e-4);
        DLIB_TEST(max(abs(mat(invstds) -mat(invstds2))) < 1e-4);
        DLIB_TEST(max(abs(mat(running_means) -mat(running_means2))) < 1e-4);
        DLIB_TEST(max(abs(mat(running_invstds) -mat(running_invstds2))) < 1e-4);

        resizable_tensor gradient_input;
        resizable_tensor src_grad, gamma_grad, beta_grad;
        resizable_tensor src_grad2, gamma_grad2, beta_grad2;
        gradient_input.copy_size(dest);
        src_grad.copy_size(src); src_grad = 0; src_grad2 = src_grad;
        gamma_grad.copy_size(gamma); gamma_grad = 0; gamma_grad2 = gamma_grad;
        beta_grad.copy_size(beta); beta_grad = 0; beta_grad2 = beta_grad;
        rnd.fill_uniform(gradient_input);


        cpu::batch_normalize_conv_gradient(gradient_input, means, invstds, src, gamma, src_grad, gamma_grad, beta_grad);
        cuda::batch_normalize_conv_gradient(gradient_input, means, invstds, src, gamma, src_grad2, gamma_grad2, beta_grad2);

        dlog << LINFO << "src_grad error:   " << max(abs(mat(src_grad)-mat(src_grad2)));
        dlog << LINFO << "gamma_grad error: " << max(abs(mat(gamma_grad)-mat(gamma_grad2)));
        dlog << LINFO << "beta_grad error:  " << max(abs(mat(beta_grad)-mat(beta_grad2)));
        DLIB_TEST(max(abs(mat(src_grad)-mat(src_grad2))) < 1e-4);
        DLIB_TEST(max(abs(mat(gamma_grad)-mat(gamma_grad2))) < 1e-4);
        DLIB_TEST(max(abs(mat(beta_grad)-mat(beta_grad2))) < 1e-4);
    }


    void test_more_ops2()
    {
        dlib::rand rnd;
        tt::tensor_rand trand;

        for (int iter = 0; iter < 100; ++iter)
        {
            print_spinner();
            resizable_tensor dest1, dest2, src1, src2;
            src1.set_size(rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1);
            dest1.copy_size(src1);
            dest2.copy_size(src1);
            src2.set_size(1,src1.k(),1,1);

            trand.fill_uniform(dest1);
            trand.fill_uniform(dest2);
            trand.fill_uniform(src1);
            trand.fill_uniform(src2);

            cpu::multiply_conv(dest1, src1, src2);
            cuda::multiply_conv(dest2, src1, src2);
            DLIB_TEST(max(abs(mat(dest1)-mat(dest2))) < 1e-5);


            // now try it using the other mode of multiply_conv
            src2.copy_size(src1);
            dest1.set_size(1,src1.k(),1,1);
            dest2.set_size(1,src1.k(),1,1);
            trand.fill_uniform(dest1);
            trand.fill_uniform(dest2);
            trand.fill_uniform(src1);
            trand.fill_uniform(src2);
            cpu::multiply_conv(dest1, src1, src2);
            cuda::multiply_conv(dest2, src1, src2);
            const float scale = max(abs(mat(dest1)));
            const float scalem = mean(abs(mat(dest1)));
            DLIB_TEST_MSG(max(abs(mat(dest1)-mat(dest2)))/scale < 1e-4 , max(abs(mat(dest1)-mat(dest2)))/scale);
            DLIB_TEST_MSG(mean(abs(mat(dest1)-mat(dest2)))/scalem < 1e-5 , mean(abs(mat(dest1)-mat(dest2)))/scalem);
        }

        for (int iter = 0; iter < 100; ++iter)
        {
            print_spinner();
            resizable_tensor dest1, dest2, src, A, B;
            src.set_size(rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1);
            dest1.copy_size(src);
            dest2.copy_size(src);
            A.set_size(1,src.k(),1,1);
            B.set_size(1,src.k(),1,1);

            trand.fill_uniform(dest1);
            trand.fill_uniform(dest2);
            trand.fill_uniform(src);
            trand.fill_uniform(A);
            trand.fill_uniform(B);

            cpu::affine_transform_conv(dest1, src, A, B);
            cuda::affine_transform_conv(dest2, src, A, B);
            DLIB_TEST(max(abs(mat(dest1)-mat(dest2))) < 1e-5);
        }

        for (int iter = 0; iter < 100; ++iter)
        {
            print_spinner();
            resizable_tensor dest1, dest2, g;
            g.set_size(rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1,
                rnd.get_random_32bit_number()%30+1);
            dest1.set_size(1,g.k(),1,1);
            dest2.set_size(1,g.k(),1,1);

            trand.fill_uniform(dest1);
            trand.fill_uniform(dest2);
            trand.fill_uniform(g);

            cpu::assign_conv_bias_gradient(dest1, g);
            cuda::assign_conv_bias_gradient(dest2, g);
            const float scale = max(abs(mat(dest1)));
            const float scalem = mean(abs(mat(dest1)));
            DLIB_TEST_MSG(max(abs(mat(dest1)-mat(dest2)))/scale < 1e-4 , max(abs(mat(dest1)-mat(dest2)))/scale);
            DLIB_TEST_MSG(mean(abs(mat(dest1)-mat(dest2)))/scalem < 1e-5 , mean(abs(mat(dest1)-mat(dest2)))/scalem);
        }

    }

#endif // DLIB_USE_CUDA

// ----------------------------------------------------------------------------------------

    void test_max_pool(
        const int window_height,
        const int window_width,
        const int stride_y,
        const int stride_x 
    )
    {
        print_spinner();
        resizable_tensor A, B, gradient_input;
        A.set_size(2,2,16,7);
        B.copy_size(A);
        gradient_input.copy_size(A);

        tt::tensor_rand rnd;
        rnd.fill_gaussian(A,0,1);
        rnd.fill_gaussian(B,0,1);
        rnd.fill_gaussian(gradient_input,0,1);


        tt::pooling mp;

        mp.setup_max_pooling(window_height,window_width,stride_y,stride_x);
        mp(A, B);

        // make sure max pooling does what it's spec says it should.
        DLIB_TEST( A.num_samples() == B.num_samples());
        DLIB_TEST( A.k() == B.k());
        DLIB_TEST( A.nr() == 1+(B.nr()-window_height%2)/stride_y);
        DLIB_TEST( A.nc() == 1+(B.nc()-window_width%2)/stride_x);
        for (long s = 0; s < A.num_samples(); ++s)
        {
            for (long k = 0; k < A.k(); ++k)
            {
                for (long r = 0; r < A.nr(); ++r)
                {
                    for (long c = 0; c < A.nc(); ++c)
                    {
                        DLIB_TEST(image_plane(A,s,k)(r,c) == max(subm_clipped(image_plane(B,s,k),
                                    centered_rect(c*stride_x,
                                                  r*stride_y,
                                                  window_width,
                                                  window_height))));
                    }
                }
            }
        }
    }

// ----------------------------------------------------------------------------------------

    void test_avg_pool(
        const int window_height,
        const int window_width,
        const int stride_y,
        const int stride_x 
    )
    {
        print_spinner();
        resizable_tensor A, B, gradient_input;
        A.set_size(2,2,16,7);
        B.copy_size(A);
        gradient_input.copy_size(A);

        tt::tensor_rand rnd;
        rnd.fill_gaussian(A,0,1);
        rnd.fill_gaussian(B,0,1);
        rnd.fill_gaussian(gradient_input,0,1);


        tt::pooling mp;

        mp.setup_avg_pooling(window_height,window_width,stride_y,stride_x);
        mp(A, B);

        // make sure avg pooling does what it's spec says it should.
        DLIB_TEST( A.num_samples() == B.num_samples());
        DLIB_TEST( A.k() == B.k());
        DLIB_TEST( A.nr() == 1+(B.nr()-window_height%2)/stride_y);
        DLIB_TEST( A.nc() == 1+(B.nc()-window_width%2)/stride_x);
        for (long s = 0; s < A.num_samples(); ++s)
        {
            for (long k = 0; k < A.k(); ++k)
            {
                for (long r = 0; r < A.nr(); ++r)
                {
                    for (long c = 0; c < A.nc(); ++c)
                    {
                        float expected = mean(subm_clipped(image_plane(B,s,k),
                                            centered_rect(c*stride_x,
                                                        r*stride_y,
                                                        window_width,
                                                        window_height)));
                        float err = abs(image_plane(A,s,k)(r,c) - expected);
                        DLIB_TEST_MSG(err < 1e-5, err << "  " << expected << "  " << image_plane(A,s,k)(r,c));
                    }
                }
            }
        }
    }

// ----------------------------------------------------------------------------------------

    void test_layers()
    {
        {
            print_spinner();
            multiply_ l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            max_pool_<3,3,1,1> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            avg_pool_<3,3,1,1> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            affine_<CONV_MODE> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            affine_<FC_MODE> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            bn_<CONV_MODE> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            bn_<FC_MODE> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            con_<3,3,3,2,2> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            con_<3,3,3,1,1>l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            con_<3,3,2,1,1> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            con_<2,1,1,1,1> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            fc_<1,FC_HAS_BIAS> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            fc_<5,FC_HAS_BIAS> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            fc_<5,FC_NO_BIAS> l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            relu_ l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            prelu_ l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            sig_ l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            htan_ l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
        {
            print_spinner();
            softmax_ l;
            DLIB_TEST_MSG(test_layer(l), test_layer(l));
        }
    }

// ----------------------------------------------------------------------------------------

    template <unsigned long n, typename SUBNET> using rcon = max_pool<2,2,2,2,relu<bn_con<con<n,5,5,1,1,SUBNET>>>>;
    template <unsigned long n, typename SUBNET> using rfc = relu<bn_fc<fc<n,FC_HAS_BIAS,SUBNET>>>;

    void test_tagging(
    )
    {
        typedef loss_multiclass_log<rfc<10,skip1<rfc<84,rfc<120,tag1<rcon<16,rcon<6,input<matrix<unsigned char>>>>>>>>>> net_type;

        net_type net;
        net_type net2(num_fc_outputs(4));

        DLIB_TEST(layer<tag1>(net).num_layers == 8);
        DLIB_TEST(layer<skip1>(net).num_layers == 8+3+3);
        DLIB_TEST(&layer<skip1>(net).get_output() == &layer<tag1>(net).get_output());
        DLIB_TEST(&layer<skip1>(net).get_output() != &layer<tag1>(net).subnet().subnet().get_output());
    }

// ----------------------------------------------------------------------------------------

    class dnn_tester : public tester
    {
    public:
        dnn_tester (
        ) :
            tester ("test_dnn",
                "Runs tests on the deep neural network tools.")
        {}

        void perform_test (
        )
        {
            test_tagging();
#ifdef DLIB_USE_CUDA
            test_conv();
            test_more_ops2();
            test_more_ops(1,1);
            test_more_ops(3,4);
            test_more_ops(4,3);
            test_more_ops(4,1);
            test_more_ops(1,4);
            test_more_ops(10000,4);
            compare_bn_gpu_and_cpu();
            compare_bn_conv_gpu_and_cpu();
            test_add();
            compare_adam();
#endif
            test_max_pool(1,1,2,3);
            test_max_pool(3,3,1,1);
            test_max_pool(3,3,2,2);
            test_max_pool(2,2,2,2);
            test_max_pool(4,5,3,1);
            test_avg_pool(1,1,2,3);
            test_avg_pool(3,3,1,1);
            test_avg_pool(3,3,2,2);
            test_avg_pool(2,2,2,2);
            test_avg_pool(4,5,3,1);
            test_tanh();
            test_softmax();
            test_sigmoid();
            test_batch_normalize();
            test_batch_normalize_conv();
            test_basic_tensor_ops();
            test_layers();
        }
    } a;

}


