// Copyright (C) 2015  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#undef DLIB_DNn_LAYERS_ABSTRACT_H_
#ifdef DLIB_DNn_LAYERS_ABSTRACT_H_

#include "tensor_abstract.h"
#include "core_abstract.h"


namespace dlib
{

// ----------------------------------------------------------------------------------------

    class SUBNET 
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This object represents a deep neural network.  In particular, it is
                the simplified interface through which layer objects interact with their
                subnetworks.  A layer's two important tasks are to (1) take outputs from its
                subnetwork and forward propagate them through itself and (2) to backwards
                propagate an error gradient through itself and onto its subnetwork.
                The idea of a subnetwork is illustrated in the following diagram:

                  +---------------------------------------------------------+
                  | loss <-- layer1 <-- layer2 <-- ... <-- layern <-- input |
                  +---------------------------------------------------------+
                                      ^                            ^
                                      \__ subnetwork for layer1 __/

                Therefore, by "subnetwork" we mean the part of the network closer to the
                input.

                Note that there is no dlib::SUBNET type.  It is shown here purely to
                document the interface layer objects expect to see when they interact
                with a network.
        !*/

    public:
        // You aren't allowed to copy subnetworks from inside a layer.
        SUBNET(const SUBNET&) = delete;
        SUBNET& operator=(const SUBNET&) = delete;

        const tensor& get_output(
        ) const;
        /*!
            ensures
                - returns the output of this subnetwork.  This is the data that the next
                  layer in the network will take as input.
                - have_same_dimensions(#get_gradient_input(), get_output()) == true
        !*/

        tensor& get_gradient_input(
        );
        /*!
            ensures
                - returns the error gradient for this subnetwork.  That is, this is the
                  error gradient that this network will use to update itself.  Therefore,
                  when performing back propagation, layers that sit on top of this
                  subnetwork write their back propagated error gradients into
                  get_gradient_input().  Or to put it another way, during back propagation,
                  layers take the contents of their get_gradient_input() and back propagate
                  it through themselves and store the results into their subnetwork's
                  get_gradient_input().
        !*/

        const NEXT_SUBNET& subnet(
        ) const;
        /*!
            ensures
                - returns the subnetwork of *this network.  With respect to the diagram
                  above, if *this was layer1 then subnet() would return the network that
                  begins with layer2.
        !*/

        NEXT_SUBNET& subnet(
        );
        /*!
            ensures
                - returns the subnetwork of *this network.  With respect to the diagram
                  above, if *this was layer1 then subnet() would return the network that
                  begins with layer2.
        !*/
    };

// ----------------------------------------------------------------------------------------

    class EXAMPLE_LAYER_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                Each layer in a deep neural network can be thought of as a function,
                f(data,parameters), that takes in a data tensor, some parameters, and
                produces an output tensor.  You create an entire deep network by composing
                these functions.  Importantly, you are able to use a wide range of
                different functions to accommodate the task you are trying to accomplish.
                Therefore, dlib includes a number of common layer types but if you want to
                define your own then you simply implement a class with the same interface
                as EXAMPLE_LAYER_.

                Note that there is no dlib::EXAMPLE_LAYER_ type.  It is shown here purely
                to document the interface that a layer object must implement.

                The central work of defining a layer is implementing the forward and backward
                methods.  When you do this you have four options:
                    - Implement the forward() and backward() methods according to the
                      specification shown below.  Do not implement forward_inplace() and
                      backward_inplace().
                    - Implement the forward() and backward() methods according to the
                      specification shown below, except exclude the computed_output
                      parameter from backward().  Doing this will allow dlib to make some
                      layers execute in-place and therefore run a little faster and use
                      less memory. Do not implement forward_inplace() and
                      backward_inplace().
                    - Implement the forward_inplace() and backward_inplace() methods
                      according to the specification shown below.  Do not implement
                      forward() and backward().  These in-place methods allow some types of
                      layers to be implemented more efficiently.
                    - Implement the forward_inplace() and backward_inplace() methods
                      according to the specification shown below, except exclude the
                      computed_output parameter from backward_inplace().  Doing this will
                      allow dlib to make some layers execute in-place and therefore run a
                      little faster and use less memory.  Do not implement forward() and
                      backward().
        !*/

    public:

        EXAMPLE_LAYER_(
        );
        /*!
            ensures
                - Default constructs this object.  This function is not required to do
                  anything in particular but it must exist, that is, it is required that
                  layer objects be default constructable. 
        !*/

        EXAMPLE_LAYER_ (
            const EXAMPLE_LAYER_& item
        );
        /*!
            ensures
                - EXAMPLE_LAYER_ objects are copy constructable
        !*/

        EXAMPLE_LAYER_(
            const some_other_layer_type& item
        );
        /*!
            ensures
                - Constructs this object from item.  This form of constructor is optional
                  but it allows you to provide a conversion from one layer type to another.
                  For example, the following code is valid only if my_layer2 can be
                  constructed from my_layer1:
                    relu<fc<my_layer1<fc<input<matrix<float>>>>>> my_dnn1;
                    relu<fc<my_layer2<fc<input<matrix<float>>>>>> my_dnn2(my_dnn1);
                  This kind of pattern is useful if you want to use one type of layer
                  during training but a different type of layer during testing since it
                  allows you to easily convert between related deep neural network types.  

                  Additionally, if you provide a constructor to build a layer from another
                  layer type you should also write your layer's deserialize() routine such
                  that it can read that other layer's serialized data in addition to your
                  own serialized data.  
        !*/

        template <typename SUBNET>
        void setup (
            const SUBNET& sub
        );
        /*!
            requires
                - SUBNET implements the SUBNET interface defined at the top of this file.
            ensures
                - performs any necessary initial memory allocations and/or sets parameters
                  to their initial values prior to learning.  Therefore, calling setup
                  destroys any previously learned parameters.  Also, typically setup()
                  would look at the dimensions of the outputs of sub and configure the
                  number of parameters in *this accordingly.
        !*/

        template <typename SUBNET>
        void forward(
            const SUBNET& sub, 
            resizable_tensor& data_output
        );
        /*!
            requires
                - SUBNET implements the SUBNET interface defined at the top of this file.
                - setup() has been called.
            ensures
                - Runs the output of the subnetwork through this layer and stores the
                  results into #data_output.  In particular, forward() can use any of the
                  outputs in sub (e.g. sub.get_output(), sub.subnet().get_output(), etc.)
                  to compute whatever it wants.
        !*/

        template <typename SUBNET>
        void backward(
            const tensor& computed_output, // this parameter is optional
            const tensor& gradient_input, 
            SUBNET& sub, 
            tensor& params_grad
        );
        /*!
            requires
                - SUBNET implements the SUBNET interface defined at the top of this file.
                - setup() has been called.
                - computed_output is the tensor resulting from calling forward(sub,computed_output).  
                  Moreover, this was the most recent call to forward().  This means that
                  forward() is allowed to cache intermediate results so they can be used
                  during the backward computation.
                - have_same_dimensions(gradient_input, computed_output) == true
                - have_same_dimensions(sub.get_gradient_input(), sub.get_output()) == true
                - have_same_dimensions(params_grad, get_layer_params()) == true
            ensures
                - This function outputs the gradients of this layer with respect to the
                  input data from sub and also with respect to this layer's parameters.
                  These gradients are stored into #sub and #params_grad, respectively. To be
                  precise, the gradients are taken of a function f(sub,get_layer_params())
                  which is defined thusly:   
                    - Recalling that computed_output is a function of both sub and get_layer_params(), 
                      since it is the result of calling forward(sub,computed_output):
                      let f(sub,get_layer_params()) == dot(computed_output, gradient_input)
                  Then we define the following gradient vectors: 
                    - PARAMETER_GRADIENT == gradient of f(sub,get_layer_params()) with
                      respect to get_layer_params(). 
                    - for all valid I:
                        - DATA_GRADIENT_I == gradient of f(sub,get_layer_params()) with
                          respect to layer<I>(sub).get_output() (recall that forward() can
                          draw inputs from the immediate sub layer, sub.subnet(), or
                          any earlier layer.  So you must consider the gradients with
                          respect to all inputs drawn from sub)
                  Finally, backward() outputs these gradients by performing:
                    - params_grad = PARAMETER_GRADIENT 
                    - for all valid I:
                        - layer<I>(sub).get_gradient_input() += DATA_GRADIENT_I
        !*/

        void forward_inplace(
            const tensor& data_input, 
            tensor& data_output
        );
        /*!
            requires
                - have_same_dimensions(data_input,data_output) == true
                - setup() has been called.
            ensures
                - Runs the data_input tensor through this layer and stores the output into
                  #data_output.
                - This function supports in-place operation, i.e. having
                  is_same_object(data_input, data_output)==true
        !*/

        void backward_inplace(
            const tensor& computed_output, // this parameter is optional
            const tensor& gradient_input,
            tensor& data_grad,
            tensor& params_grad
        );
        /*!
            requires
                - setup() has been called.
                - computed_output is the tensor resulting from the most recent call to
                  forward_inplace().  This means that forward_inplace() is allowed to cache
                  intermediate results so they can be used during the backward computation.
                - have_same_dimensions(gradient_input, data_grad) == true
                - have_same_dimensions(gradient_input, computed_output) == true
                - have_same_dimensions(params_grad, get_layer_params()) == true
            ensures
                - This function supports in-place operation, i.e. having
                  is_same_object(gradient_input, data_grad)==true
                - This function outputs the gradients of this layer with respect to the
                  input data from a sublayer and also with respect to this layer's parameters.
                  These gradients are stored into #data_grad and #params_grad, respectively. To be
                  precise, the gradients are taken of a function f(data_input,get_layer_params())
                  which is defined thusly:   
                    - Recalling that computed_output is a function of both the input to
                      forward_inplace() and get_layer_params(), since it is the result of
                      calling forward_inplace(data_input,computed_output):
                      let f(data_input,get_layer_params()) == dot(computed_output, gradient_input)
                  Then we define the following gradient vectors: 
                    - PARAMETER_GRADIENT == gradient of f(data_input,get_layer_params()) with
                      respect to get_layer_params(). 
                    - DATA_GRADIENT == gradient of f(data_input,get_layer_params()) with respect
                      to data_input. 
                  Finally, backward_inplace() outputs these gradients by performing:
                    - params_grad = PARAMETER_GRADIENT 
                    - data_grad = DATA_GRADIENT
        !*/

        const tensor& get_layer_params(
        ) const; 
        /*!
            ensures
                - returns the parameters that define the behavior of forward().
        !*/

        tensor& get_layer_params(
        ); 
        /*!
            ensures
                - returns the parameters that define the behavior of forward().
        !*/

    };

    void serialize(const EXAMPLE_LAYER_& item, std::ostream& out);
    void deserialize(EXAMPLE_LAYER_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    // For each layer you define, always define an add_layer template so that layers can be
    // easily composed.  Moreover, the convention is that the layer class ends with an _
    // while the add_layer template has the same name but without the trailing _.
    template <typename SUBNET>
    using EXAMPLE_LAYER = add_layer<EXAMPLE_LAYER_, SUBNET>;

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    enum fc_bias_mode
    {
        FC_HAS_BIAS = 0,
        FC_NO_BIAS = 1
    };

    struct num_fc_outputs
    {
        num_fc_outputs(unsigned long n) : num_outputs(n) {}
        unsigned long num_outputs;
    };

    template <
        unsigned long num_outputs,
        fc_bias_mode bias_mode
        >
    class fc_
    {
        /*!
            REQUIREMENTS ON num_outputs
                num_outputs > 0

            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a fully connected layer that takes an input
                tensor and multiplies it by a weight matrix and outputs the results.
        !*/

    public:

        fc_(
        );
        /*!
            ensures
                - #get_num_outputs() == num_outputs
                - #get_bias_mode() == bias_mode 
        !*/

        unsigned long get_num_outputs (
        ) const; 
        /*!
            ensures
                - This layer outputs column vectors that contain get_num_outputs()
                  elements. That is, the output tensor T from forward() will be such that:
                    - T.num_samples() == however many samples were given to forward().
                    - T.k() == get_num_outputs()
                    - The rest of the dimensions of T will be 1.
        !*/

        fc_bias_mode get_bias_mode (
        ) const;
        /*!
            ensures
                - returns the bias mode which determines if this layer includes bias terms.
                  That is, if the bias mode is FC_HAS_BIAS then a different constant scalar
                  is added to each of the outputs of this layer. 
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        template <typename SUBNET> void forward(const SUBNET& sub, resizable_tensor& output);
        template <typename SUBNET> void backward(const tensor& gradient_input, SUBNET& sub, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
        !*/

        friend void serialize(const fc_& item, std::ostream& out);
        friend void deserialize(fc_& item, std::istream& in);
        /*!
            provides serialization support  
        !*/
    };


    template <
        unsigned long num_outputs,
        fc_bias_mode bias_mode,
        typename SUBNET
        >
    using fc = add_layer<fc_<num_outputs,bias_mode>, SUBNET>;

// ----------------------------------------------------------------------------------------

    template <
        long _num_filters,
        long _nr,
        long _nc,
        int _stride_y,
        int _stride_x
        >
    class con_
    {
        /*!
            REQUIREMENTS ON TEMPLATE ARGUMENTS
                All of them must be > 0.

            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a convolution layer that takes an input tensor
                (nominally representing an image) and convolves it with a set of filters
                and then outputs the results. 

                The dimensions of the tensors output by this layer are as follows (letting
                IN be the input tensor and OUT the output tensor):
                    - OUT.num_samples() == IN.num_samples()
                    - OUT.k()  == num_filters()
                    - OUT.nr() == 1+(IN.nr()-nr()%2)/stride_y()
                    - OUT.nc() == 1+(IN.nc()-nc()%2)/stride_x()
        !*/

    public:
        con_(
        );
        /*!
            ensures
                - #num_filters() == _num_filters
                - #nr() == _nr
                - #nc() == _nc
                - #stride_y() == _stride_y
                - #stride_x() == _stride_x
        !*/

        long num_filters(
        ) const; 
        /*!
            ensures
                - returns the number of filters contained in this layer.  The k dimension
                  of the output tensors produced by this layer will be equal to the number
                  of filters.
        !*/

        long nr(
        ) const; 
        /*!
            ensures
                - returns the number of rows in the filters in this layer.
        !*/

        long nc(
        ) const;
        /*!
            ensures
                - returns the number of columns in the filters in this layer.
        !*/

        long stride_y(
        ) const; 
        /*!
            ensures
                - returns the vertical stride used when convolving the filters over an
                  image.  That is, each filter will be moved stride_y() pixels down at a
                  time when it moves over the image.
        !*/

        long stride_x(
        ) const;
        /*!
            ensures
                - returns the horizontal stride used when convolving the filters over an
                  image.  That is, each filter will be moved stride_x() pixels right at a
                  time when it moves over the image.
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        template <typename SUBNET> void forward(const SUBNET& sub, resizable_tensor& output);
        template <typename SUBNET> void backward(const tensor& gradient_input, SUBNET& sub, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
        !*/

        friend void serialize(const con_& item, std::ostream& out);
        friend void deserialize(con_& item, std::istream& in);
        /*!
            provides serialization support  
        !*/

    };

    template <
        long num_filters,
        long nr,
        long nc,
        int stride_y,
        int stride_x,
        typename SUBNET
        >
    using con = add_layer<con_<num_filters,nr,nc,stride_y,stride_x>, SUBNET>;

// ----------------------------------------------------------------------------------------

    class dropout_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a dropout layer.  Therefore, it passes its inputs
                through the stochastic function f(x) which outputs either 0 or x.  The
                probability of 0 being output is given by the drop_rate argument to this
                object's constructor.

                Note that, after you finish training a network with dropout, it is a good
                idea to replace each dropout_ layer with a multiply_ layer because the
                multiply_ layer is faster and deterministic. 
        !*/

    public:

        explicit dropout_(
            float drop_rate = 0.5
        );
        /*!
            requires
                - 0 <= drop_rate <= 1
            ensures
                - #get_drop_rate() == drop_rate
        !*/

        float get_drop_rate (
        ) const; 
        /*!
            ensures
                - returns the probability that an individual input value to this layer will
                  be replaced with 0.
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        void forward_inplace(const tensor& input, tensor& output);
        void backward_inplace(const tensor& gradient_input, tensor& data_grad, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
        !*/
    };

    void serialize(const dropout_& item, std::ostream& out);
    void deserialize(dropout_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET>
    using dropout = add_layer<dropout_, SUBNET>;

// ----------------------------------------------------------------------------------------

    class multiply_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a basic layer that just multiplies its input
                tensor with a constant value and returns the result.  It therefore has no
                learnable parameters.
        !*/

    public:
        explicit multiply_(
            float val = 0.5
        ); 
        /*!
            ensures
                - #get_multiply_value() == val
        !*/

        multiply_ (
            const dropout_& item
        ); 
        /*!
            ensures
                - #get_multiply_value() == 1-item.get_drop_rate()
                  (i.e. We construct the multiply_ layer so that it is essentially a
                  deterministic version of the given dropout_ layer)
        !*/

        float get_multiply_value (
        ) const;
        /*!
            ensures
                - this layer simply multiplies its input tensor by get_multiply_value() and
                  produces the result as output.
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        void forward_inplace(const tensor& input, tensor& output);
        void backward_inplace(const tensor& gradient_input, tensor& data_grad, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
        !*/
    };

    void serialize(const multiply_& item, std::ostream& out);
    void deserialize(multiply_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET>
    using multiply = add_layer<multiply_, SUBNET>;

// ----------------------------------------------------------------------------------------

    enum layer_mode
    {
        CONV_MODE = 0, // convolutional mode
        FC_MODE = 1    // fully connected mode
    };

    template <
        layer_mode mode
        >
    class bn_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a batch normalization layer that implements the
                method described in the paper: 
                    Batch Normalization: Accelerating Deep Network Training by Reducing
                    Internal Covariate Shift by Sergey Ioffe and Christian Szegedy
                
                In particular, this layer produces output tensors with the same
                dimensionality as the input tensors, except that the mean and variances of
                the elements have been standardized to 0 and 1 respectively. 

                It should also be noted that when tensors with a num_samples() dimension of
                1 are passed to this layer it doesn't perform batch normalization.
                Instead, it runs in "inference mode" where the learned linear normalizing
                transformation is used to transform the tensor. 

                Finally, after you finish training a batch normalized network, it is a good
                idea to replace each bn_ layer with an affine_ layer because the affine_
                layer is faster and will never surprise you by performing batch
                normalization on tensors that have a num_samples() dimension > 1.  This allows
                you to run large mini-batches of samples through your final network without
                batch normalization executing at all. 
        !*/

    public:
        bn_(
        );
        /*!
            ensures
                - #get_mode() == mode
                - get_running_stats_window_size() == 1000
        !*/

        explicit bn_(
            unsigned long window_size
        );
        /*!
            ensures
                - #get_mode() == mode 
                - get_running_stats_window_size() == window_size
        !*/

        layer_mode get_mode(
        ) const; 
        /*!
            ensures
                - returns the mode of this layer, either CONV_MODE or FC_MODE.
                  If the mode is FC_MODE then the normalization is applied across the
                  samples in a tensor (i.e. k()*nr()*nc() different things will be
                  normalized).  Otherwise, normalization is applied across everything
                  except for the k() dimension, resulting in there being only k()
                  normalization equations that are applied spatially over the tensor.

                  Therefore, if you are putting batch normalization after a fully connected
                  layer you should use FC_MODE.  Otherwise, if you are putting batch
                  normalization after a convolutional layer you should use CONV_MODE.
        !*/

        unsigned long get_running_stats_window_size (
        ) const; 
        /*!
            ensures
                - Just as recommended in the batch normalization paper, this object keeps a
                  running average of the mean and standard deviations of the features.
                  These averages are used during "inference mode" so you can run a single
                  object through a batch normalized network.  They are also what is used to
                  initialize an affine_ layer that is constructed from a bn_ layer.  This
                  function returns the effective number of recent samples used to compute
                  the running average.
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        template <typename SUBNET> void forward(const SUBNET& sub, resizable_tensor& output);
        template <typename SUBNET> void backward(const tensor& gradient_input, SUBNET& sub, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
        !*/

        friend void serialize(const bn_& item, std::ostream& out);
        friend void deserialize(bn_& item, std::istream& in);
        /*!
            provides serialization support  
        !*/

    };

    template <typename SUBNET>
    using bn_con = add_layer<bn_<CONV_MODE>, SUBNET>;
    template <typename SUBNET>
    using bn_fc = add_layer<bn_<FC_MODE>, SUBNET>;

// ----------------------------------------------------------------------------------------

    template <
        layer_mode mode
        >
    class affine_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it applies a simple pointwise linear transformation to an
                input tensor.  You can think of it as having two parameter tensors, A and
                B.  If the input tensor is called INPUT then the output of this layer is:
                    A*INPUT+B
                where all operations are performed element wise and each sample in the
                INPUT tensor is processed separately.

                Moreover, this object has two modes that effect the dimensionalities of A
                and B and how they are applied to compute A*INPUT+B.  If
                get_mode()==FC_MODE then A and B each have the same dimensionality as the
                input tensor, except their num_samples() dimensions are 1.  If
                get_mode()==CONV_MODE then A and B have all their dimensions set to 1
                except for k(), which is equal to INPUT.k().

                In either case, the computation of A*INPUT+B is performed pointwise over all
                the elements of INPUT using either:
                    OUTPUT(n,k,r,c) == A(1,k,r,c)*INPUT(n,k,r,c)+B(1,k,r,c)
                or
                    OUTPUT(n,k,r,c) == A(1,k,1,1)*INPUT(n,k,r,c)+B(1,k,1,1)
                as appropriate.


                Finally, note that the parameters of this layer are not learnable and
                therefore not modified during network updates.  Instead, the layer will
                perform the identity transformation unless it is initialized with a bn_
                layer, in which case it will perform whatever transformation the bn_ layer
                has learned.
        !*/

    public:

        affine_(
        );
        /*!
            ensures
                - #get_mode() == mode
        !*/

        affine_(
            const bn_<mode>& layer
        );
        /*!
            ensures
                - Constructs affine_ so that it performs the same transformation as the
                  supplied batch normalization layer.  You would want to do this after you
                  finish training a network with bn_ layers because the affine_ layer will
                  execute faster.  
                - #get_mode() == layer.get_mode()
        !*/

        layer_mode get_mode(
        ) const; 
        /*!
            ensures
                - returns the mode of this layer, either CONV_MODE or FC_MODE.  
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        void forward_inplace(const tensor& input, tensor& output);
        void backward_inplace(const tensor& computed_output, const tensor& gradient_input, tensor& data_grad, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
            Also note that get_layer_params() always returns an empty tensor since there
            are no learnable parameters in this object.
        !*/

        friend void serialize(const affine_& item, std::ostream& out);
        friend void deserialize(affine_& item, std::istream& in);
        /*!
            provides serialization support  
        !*/
    };

    template <typename SUBNET>
    using affine_con = add_layer<affine_<CONV_MODE>, SUBNET>;
    template <typename SUBNET>
    using affine_fc = add_layer<affine_<FC_MODE>, SUBNET>;

// ----------------------------------------------------------------------------------------

    template <
        long _nr,
        long _nc,
        int _stride_y,
        int _stride_x
        >
    class max_pool_
    {
        /*!
            REQUIREMENTS ON TEMPLATE ARGUMENTS
                All of them must be > 0.

            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a max pooling layer that takes an input tensor
                and downsamples it.  It does this by sliding a window over the images in an
                input tensor and outputting, for each channel, the maximum element within
                the window.  

                To be precise, if we call the input tensor IN and the output tensor OUT,
                then OUT is defined as follows:
                    - OUT.num_samples() == IN.num_samples()
                    - OUT.k()  == IN.k()
                    - OUT.nr() == 1+(IN.nr()-nr()%2)/stride_y()
                    - OUT.nc() == 1+(IN.nc()-nc()%2)/stride_x()
                    - for all valid s, k, r, and c:
                        - image_plane(OUT,s,k)(r,c) == max(subm_clipped(image_plane(IN,s,k),
                                                                        r*stride_y(),
                                                                        c*stride_x(),
                                                                        nr(),
                                                                        nc()))
        !*/

    public:

        max_pool_ (
        );
        /*!
            ensures
                - #nr() == _nr
                - #nc() == _nc
                - #stride_y() == _stride_y
                - #stride_x() == _stride_x
        !*/

        long nr(
        ) const; 
        /*!
            ensures
                - returns the number of rows in the max pooling window.
        !*/

        long nc(
        ) const;
        /*!
            ensures
                - returns the number of columns in the max pooling window.
        !*/

        long stride_y(
        ) const; 
        /*!
            ensures
                - returns the vertical stride used when scanning the max pooling window
                  over an image.  That is, each window will be moved stride_y() pixels down
                  at a time when it moves over the image.
        !*/

        long stride_x(
        ) const;
        /*!
            ensures
                - returns the horizontal stride used when scanning the max pooling window
                  over an image.  That is, each window will be moved stride_x() pixels down
                  at a time when it moves over the image.
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        template <typename SUBNET> void forward(const SUBNET& sub, resizable_tensor& output);
        template <typename SUBNET> void backward(const tensor& computed_output, const tensor& gradient_input, SUBNET& sub, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface. 
            Note that this layer doesn't have any parameters, so the tensor returned by
            get_layer_params() is always empty.
        !*/

        friend void serialize(const max_pool_& item, std::ostream& out);
        friend void deserialize(max_pool_& item, std::istream& in);
        /*!
            provides serialization support  
        !*/
    };

    template <
        long nr,
        long nc,
        int stride_y,
        int stride_x,
        typename SUBNET
        >
    using max_pool = add_layer<max_pool_<nr,nc,stride_y,stride_x>, SUBNET>;

// ----------------------------------------------------------------------------------------

    template <
        long _nr,
        long _nc,
        int _stride_y,
        int _stride_x
        >
    class avg_pool_
    {
        /*!
            REQUIREMENTS ON TEMPLATE ARGUMENTS
                All of them must be > 0.

            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines an average pooling layer that takes an input tensor
                and downsamples it.  It does this by sliding a window over the images in an
                input tensor and outputting, for each channel, the average element within
                the window.  

                To be precise, if we call the input tensor IN and the output tensor OUT,
                then OUT is defined as follows:
                    - OUT.num_samples() == IN.num_samples()
                    - OUT.k()  == IN.k()
                    - OUT.nr() == 1+(IN.nr()-nr()%2)/stride_y()
                    - OUT.nc() == 1+(IN.nc()-nc()%2)/stride_x()
                    - for all valid s, k, r, and c:
                        - image_plane(OUT,s,k)(r,c) == mean(subm_clipped(image_plane(IN,s,k),
                                                                        r*stride_y(),
                                                                        c*stride_x(),
                                                                        nr(),
                                                                        nc()))
        !*/

    public:

        avg_pool_ (
        );
        /*!
            ensures
                - #nr() == _nr
                - #nc() == _nc
                - #stride_y() == _stride_y
                - #stride_x() == _stride_x
        !*/

        long nr(
        ) const; 
        /*!
            ensures
                - returns the number of rows in the pooling window.
        !*/

        long nc(
        ) const;
        /*!
            ensures
                - returns the number of columns in the pooling window.
        !*/

        long stride_y(
        ) const; 
        /*!
            ensures
                - returns the vertical stride used when scanning the pooling window
                  over an image.  That is, each window will be moved stride_y() pixels down
                  at a time when it moves over the image.
        !*/

        long stride_x(
        ) const;
        /*!
            ensures
                - returns the horizontal stride used when scanning the pooling window
                  over an image.  That is, each window will be moved stride_x() pixels down
                  at a time when it moves over the image.
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        template <typename SUBNET> void forward(const SUBNET& sub, resizable_tensor& output);
        template <typename SUBNET> void backward(const tensor& computed_output, const tensor& gradient_input, SUBNET& sub, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface. 
            Note that this layer doesn't have any parameters, so the tensor returned by
            get_layer_params() is always empty.
        !*/

        friend void serialize(const avg_pool_& item, std::ostream& out);
        friend void deserialize(avg_pool_& item, std::istream& in);
        /*!
            provides serialization support  
        !*/
    };

    template <
        long nr,
        long nc,
        int stride_y,
        int stride_x,
        typename SUBNET
        >
    using avg_pool = add_layer<avg_pool_<nr,nc,stride_y,stride_x>, SUBNET>;

// ----------------------------------------------------------------------------------------

    class relu_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a rectified linear layer.  Therefore, it passes
                its inputs through the function 
                    f(x)=max(x,0) 
                where f() is applied pointwise across the input tensor.
        !*/

    public:

        relu_(
        );

        template <typename SUBNET> void setup (const SUBNET& sub);
        void forward_inplace(const tensor& input, tensor& output);
        void backward_inplace(const tensor& computed_output, const tensor& gradient_input, tensor& data_grad, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
            Note that this layer doesn't have any parameters, so the tensor returned by
            get_layer_params() is always empty.
        !*/
    };

    void serialize(const relu_& item, std::ostream& out);
    void deserialize(relu_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET>
    using relu = add_layer<relu_, SUBNET>;

// ----------------------------------------------------------------------------------------

    class prelu_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a parametric rectified linear layer.  Therefore,
                it passes its inputs through the function 
                    f(x) = x>0 ? x : p*x 
                where f() is applied pointwise across the input tensor and p is a scalar
                parameter learned by this layer.


                This is the layer type introduced in the paper:
                    He, Kaiming, et al. "Delving deep into rectifiers: Surpassing
                    human-level performance on imagenet classification." Proceedings of the
                    IEEE International Conference on Computer Vision. 2015.
        !*/

    public:

        explicit prelu_(
            float initial_param_value = 0.25
        );
        /*!
            ensures
                - The p parameter will be initialized with initial_param_value.
                - #get_initial_param_value() == initial_param_value.
        !*/

        float get_initial_param_value (
        ) const;
        /*!
            ensures
                - returns the initial value of the prelu parameter. 
        !*/

        template <typename SUBNET> void setup (const SUBNET& sub);
        void forward_inplace(const tensor& input, tensor& output);
        void backward_inplace(const tensor& computed_output, const tensor& gradient_input, tensor& data_grad, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
        !*/
    };

    void serialize(const prelu_& item, std::ostream& out);
    void deserialize(prelu_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET>
    using prelu = add_layer<prelu_, SUBNET>;

// ----------------------------------------------------------------------------------------

    class sig_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a sigmoid layer.  Therefore, it passes its inputs
                through the function 
                    f(x)=1/(1+exp(-x)) 
                where f() is applied pointwise across the input tensor.
        !*/

    public:

        sig_(
        );

        template <typename SUBNET> void setup (const SUBNET& sub);
        void forward_inplace(const tensor& input, tensor& output);
        void backward_inplace(const tensor& computed_output, const tensor& gradient_input, tensor& data_grad, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
            Note that this layer doesn't have any parameters, so the tensor returned by
            get_layer_params() is always empty.
        !*/
    };

    void serialize(const sig_& item, std::ostream& out);
    void deserialize(sig_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET>
    using sig = add_layer<sig_, SUBNET>;

// ----------------------------------------------------------------------------------------

    class htan_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a hyperbolic tangent layer.  Therefore, it passes
                its inputs through the function 
                    f(x)=std::tanh(x)
                where f() is applied pointwise across the input tensor.
        !*/

    public:

        htan_(
        );

        template <typename SUBNET> void setup (const SUBNET& sub);
        void forward_inplace(const tensor& input, tensor& output);
        void backward_inplace(const tensor& computed_output, const tensor& gradient_input, tensor& data_grad, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
            Note that this layer doesn't have any parameters, so the tensor returned by
            get_layer_params() is always empty.
        !*/
    };

    void serialize(const htan_& item, std::ostream& out);
    void deserialize(htan_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET>
    using htan = add_layer<htan_, SUBNET>;

// ----------------------------------------------------------------------------------------

    class softmax_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                In particular, it defines a softmax layer.  To be precise, we define the
                softmax function s(x) as:
                    s(x) == exp(x)/sum(exp(x)) 
                where x is a vector.  Then this layer treats its input tensor as a
                collection of multi-channel images and applies s() to each spatial location
                in each image.  In each application, the tensor::k() channel elements at
                each position are input to s() and then replaced by the outputs of s().   

                This means that, for example, if you collapsed each output image to a 1
                channel image by adding the channels then you would end up with images
                where each pixel value was 1.  This is because the sum of the outputs of
                s() will always be equal to 1.
        !*/

    public:

        softmax_(
        );

        template <typename SUBNET> void setup (const SUBNET& sub);
        void forward_inplace(const tensor& input, tensor& output);
        void backward_inplace(const tensor& computed_output, const tensor& gradient_input, tensor& data_grad, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
            Note that this layer doesn't have any parameters, so the tensor returned by
            get_layer_params() is always empty.
        !*/
    };

    void serialize(const softmax_& item, std::ostream& out);
    void deserialize(softmax_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET>
    using softmax = add_layer<softmax_, SUBNET>;

// ----------------------------------------------------------------------------------------

    template <
        template<typename> class tag
        >
    class add_prev_
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an implementation of the EXAMPLE_LAYER_ interface defined above.
                This layer simply adds the output of two previous layers.  In particular,
                it adds the tensor from its immediate predecessor layer, sub.get_output(),
                with the tensor from a deeper layer, layer<tag>(sub).get_output().

                Therefore, you supply a tag via add_prev_'s template argument that tells it
                what layer to add to the output of the previous layer.  The result of this
                addition is output by add_prev_.  Finally, the addition happens pointwise
                according to 4D tensor arithmetic.  If the dimensions don't match then
                missing elements are presumed to be equal to 0.
        !*/

    public:
        add_prev_(
        ); 

        template <typename SUBNET> void setup (const SUBNET& sub);
        template <typename SUBNET> void forward(const SUBNET& sub, resizable_tensor& output);
        template <typename SUBNET> void backward(const tensor& gradient_input, SUBNET& sub, tensor& params_grad);
        const tensor& get_layer_params() const; 
        tensor& get_layer_params(); 
        /*!
            These functions are implemented as described in the EXAMPLE_LAYER_ interface.
        !*/
    };

    void serialize(const add_prev_& item, std::ostream& out);
    void deserialize(add_prev_& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <
        template<typename> class tag,
        typename SUBNET
        >
    using add_prev = add_layer<add_prev_<tag>, SUBNET>;

    // Here we add some convenient aliases for using add_prev_ with the tag layers. 
    template <typename SUBNET> using add_prev1  = add_prev<tag1, SUBNET>;
    template <typename SUBNET> using add_prev2  = add_prev<tag2, SUBNET>;
    template <typename SUBNET> using add_prev3  = add_prev<tag3, SUBNET>;
    template <typename SUBNET> using add_prev4  = add_prev<tag4, SUBNET>;
    template <typename SUBNET> using add_prev5  = add_prev<tag5, SUBNET>;
    template <typename SUBNET> using add_prev6  = add_prev<tag6, SUBNET>;
    template <typename SUBNET> using add_prev7  = add_prev<tag7, SUBNET>;
    template <typename SUBNET> using add_prev8  = add_prev<tag8, SUBNET>;
    template <typename SUBNET> using add_prev9  = add_prev<tag9, SUBNET>;
    template <typename SUBNET> using add_prev10 = add_prev<tag10, SUBNET>;
    using add_prev1_  = add_prev_<tag1>;
    using add_prev2_  = add_prev_<tag2>;
    using add_prev3_  = add_prev_<tag3>;
    using add_prev4_  = add_prev_<tag4>;
    using add_prev5_  = add_prev_<tag5>;
    using add_prev6_  = add_prev_<tag6>;
    using add_prev7_  = add_prev_<tag7>;
    using add_prev8_  = add_prev_<tag8>;
    using add_prev9_  = add_prev_<tag9>;
    using add_prev10_ = add_prev_<tag10>;

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_DNn_LAYERS_ABSTRACT_H_

