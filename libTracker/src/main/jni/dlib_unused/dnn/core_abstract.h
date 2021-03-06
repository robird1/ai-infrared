// Copyright (C) 2015  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#undef DLIB_DNn_CORE_ABSTRACT_H_
#ifdef DLIB_DNn_CORE_ABSTRACT_H_

#include "tensor_abstract.h"
#include <memory>
#include <type_traits>
#include <tuple>
#include <vector>
#include "../rand.h"


namespace dlib
{

// ----------------------------------------------------------------------------------------

    void randomize_parameters (
        tensor& params,
        unsigned long num_inputs_and_outputs,
        dlib::rand& rnd
    );
    /*!
        ensures
            - This function assigns random values into params based on the given random
              number generator.  In particular, it uses the parameter initialization method
              of formula 16 from the paper "Understanding the difficulty of training deep
              feedforward neural networks" by Xavier Glorot and Yoshua Bengio.
            - It is assumed that the total number of inputs and outputs from the layer is
              num_inputs_and_outputs.  That is, you should set num_inputs_and_outputs to
              the sum of the dimensionalities of the vectors going into and out of the
              layer that uses params as its parameters.
    !*/

    template <
        typename... T 
        >
    auto tuple_tail(
        const std::tuple<T...>& item 
    );
    /*!
        ensures
            - returns a tuple that contains everything in item except for tuple_head(item).
              The items will be in the same order as they are in item, just without
              tuple_head(item).
            - This function will correctly handle nested tuples.
    !*/

    template <typename... T>
    auto tuple_head (
        const std::tuple<T...>& item
    ); 
    /*!
        ensures
            - returns a copy of the first thing in the tuple that isn't a std::tuple.
              Essentially, this function calls std::get<0>() recursively on item until
              a non-std::tuple object is found.
    !*/

    double log1pexp(
        double x
    );
    /*!
        ensures
            - returns log(1+exp(x))
              (except computes it using a numerically accurate method)
    !*/

// ----------------------------------------------------------------------------------------

    bool dnn_prefer_fastest_algorithms(
    );
    /*!
        ensures
            - If dlib should prefer to use fast algorithms rather than ones that use less
              RAM then this function returns true and false otherwise.
            - On program startup this function will default to true.
    !*/

    void set_dnn_prefer_fastest_algorithms(
    );
    /*!
        ensures
            - #dnn_prefer_fastest_algorithms() == true
    !*/

    void set_dnn_prefer_smallest_algorithms(
    );
    /*!
        ensures
            - #dnn_prefer_fastest_algorithms() == false 
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename T
        >
    class sstack
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is a basic stack of T objects.  It contains no data itself but simply
                points to a memory range of T object and allows you to access that block of
                T objects as a stack.
        !*/

    public:
        typedef T value_type;

        sstack() = delete;

        sstack (
            T* data,
            size_t s
        );
        /*!
            ensures
                - #size() == s
                - #top() == *data
                - #pop(i).top() == data[i]
        !*/

        const T& top(
        ) const;
        /*!
            requires
                - size() != 0
            ensures
                - returns the top element of the stack.
        !*/

        T& top(
        );
        /*!
            requires
                - size() != 0
            ensures
                - returns the top element of the stack.  
        !*/

        size_t size(
        ) const;
        /*!
            ensures
                - returns the number of elements in this stack.  
        !*/

        sstack pop(
            size_t num = 1
        ); 
        /*!
            requires
                - num < size()
            ensures
                - returns a reference to the sub-stack S such that:
                    - S.size() == size()-num.
                    - S.top() is num elements down the stack. 
        !*/
    };

    template <
        typename T
        >
    sstack<T> make_sstack(
        std::vector<T>& item
    ) { return sstack<T>(item.data(), item.size()); }
    /*!
        ensures
            - returns a sstack that sits on top of the given std::vector.
    !*/

// ----------------------------------------------------------------------------------------

    template <
        typename LAYER_DETAILS, 
        typename SUBNET
        >
    class add_layer
    {
        /*!
            REQUIREMENTS ON LAYER_DETAILS
                - Must be a type that implements the EXAMPLE_LAYER_ interface defined in
                  layers_abstract.h

            REQUIREMENTS ON SUBNET
                - One of the following must be true:
                    - SUBNET implements the EXAMPLE_INPUT_LAYER interface defined in
                      input_abstract.h.
                    - SUBNET is an add_layer object.
                    - SUBNET is an add_tag_layer object.
                    - SUBNET is an add_skip_layer object.
                    - SUBNET is a repeat object.

            WHAT THIS OBJECT REPRESENTS
                This object represents a deep neural network.  In particular, it is a tool
                for adding another layer on top of the neural network of type SUBNET, which
                is specified as a template argument.  The specific layer added is defined
                by the LAYER_DETAILS details template argument.
        !*/

    public:
        typedef LAYER_DETAILS layer_details_type;
        typedef SUBNET subnet_type;
        typedef typename subnet_type::input_type input_type;
        const static unsigned int sample_expansion_factor = subnet_type::sample_expansion_factor;
        // If SUBNET is an input layer then num_layers == 1, otherwise it has the
        // definition shown here:
        const static size_t num_layers = subnet_type::num_layers + 1;

        add_layer(
        );
        /*!
            ensures
                - default constructs all the layers in this network.
        !*/

        add_layer(const add_layer&) = default;
        add_layer(add_layer&&) = default;
        add_layer& operator=(add_layer&&) = default;
        add_layer& operator=(const add_layer&) = default;
        /*!
            ensures
                - this object is copyable and movable.
        !*/

        template <typename T, typename U>
        add_layer(
            const add_layer<T,U>& item
        );
        /*!
            ensures
                - This constructor allows you to copy neural network objects from one to
                  another as long as their corresponding layers can be constructed from
                  each other.
                - #layer_details() == layer_details_type(item.layer_details())
                - #subnet()        == subnet_type(item.subnet())
        !*/

        template <typename ...T, typename LD, typename ...U>
        add_layer(
            const std::tuple<LD,U...>& layer_det, 
            T&& ...args
        );
        /*!
            ensures
                - #layer_details() == layer_details_type(tuple_head(layer_det))
                - #subnet()        == subnet_type(tuple_tail(layer_det),args)
        !*/

        template <typename ...T>
        add_layer(
            const layer_details_type& layer_det, 
            T&& ...args
        );
        /*!
            ensures
                - #layer_details() == layer_details_type(layer_det)
                - #subnet()        == subnet_type(args)
        !*/

        template <typename ...T>
        add_layer(
            T&& ...args
        );
        /*!
            ensures
                - This version of the constructor is only called if layer_details_type
                  can't be constructed from the first thing in args.  In this case, the
                  args are simply passed on to the sub layers in their entirety.
                - #layer_details() == layer_details_type()
                - #subnet()        == subnet_type(args)
        !*/

        template <typename ...T>
        add_layer(
            layer_details_type&& layer_det, 
            T&& ...args
        );
        /*!
            ensures
                - #layer_details() == layer_det
                - #subnet()        == subnet_type(args)
        !*/

        template <typename input_iterator>
        void to_tensor (
            input_iterator ibegin,
            input_iterator iend,
            resizable_tensor& data
        ) const;
        /*!
            requires
                - [ibegin, iend) is an iterator range over input_type objects.
                - std::distance(ibegin,iend) > 0
            ensures
                - Converts the iterator range into a tensor and stores it into #data.
                - #data.num_samples() == distance(ibegin,iend)*sample_expansion_factor. 
                - The data in the ith sample of #data corresponds to the input_type object
                  *(ibegin+i/sample_expansion_factor).
                - Invokes data.async_copy_to_device() so that the data begins transferring
                  to the GPU device, if present.
                - This function is implemented by calling the to_tensor() routine defined
                  at the input layer of this network.  
        !*/

        const subnet_type& subnet(
        ) const; 
        /*!
            ensures
                - returns the immediate subnetwork of *this network.  
        !*/

        subnet_type& subnet(
        );
        /*!
            ensures
                - returns the immediate subnetwork of *this network.  
        !*/

        const layer_details_type& layer_details(
        ) const; 
        /*!
            ensures
                - returns the layer_details_type instance that defines the behavior of the
                  layer at the top of this network.  I.e. returns the layer details that
                  defines the behavior of the layer nearest to the network output rather
                  than the input layer.
        !*/

        layer_details_type& layer_details(
        );
        /*!
            ensures
                - returns the layer_details_type instance that defines the behavior of the
                  layer at the top of this network.  I.e. returns the layer details that
                  defines the behavior of the layer nearest to the network output rather
                  than the input layer.
        !*/

        template <typename input_iterator>
        const tensor& operator() (
            input_iterator ibegin,
            input_iterator iend
        );
        /*!
            requires
                - [ibegin, iend) is an iterator range over input_type objects.
                - std::distance(ibegin,iend) > 0
            ensures
                - runs [ibegin,iend) through the network and returns the results.
                  In particular, this function performs:
                    to_tensor(ibegin,iend,temp_tensor);
                    return forward(temp_tensor);
                - The return value from this function is also available in #get_output().
                  i.e. this function returns #get_output().
                - have_same_dimensions(#get_gradient_input(), #get_output()) == true.
                - All elements of #get_gradient_input() are set to 0. 
                  i.e. calling this function clears out #get_gradient_input() and ensures
                  it has the same dimensions as the most recent output.
        !*/

        const tensor& operator() (
            const input_type& x
        );
        /*!
            ensures
                - runs a single x through the network and returns the output.
                  I.e. returns (*this)(&x, &x+1);
        !*/

        const tensor& forward(
            const tensor& x
        );
        /*!
            requires
                - x.num_samples()%sample_expansion_factor == 0
                - x.num_samples() > 0
            ensures
                - Runs x through the network and returns the results.  In particular, this
                  function performs the equivalent of:
                    subnet().forward(x);
                    if (this is the first time forward() has been called) then
                        layer_details().setup(subnet());
                    layer_details().forward(subnet(), get_output());
                - The return value from this function is also available in #get_output().
                  i.e. this function returns #get_output().
                - have_same_dimensions(#get_gradient_input(), #get_output()) == true
                - All elements of #get_gradient_input() are set to 0. 
                  i.e. calling this function clears out #get_gradient_input() and ensures
                  it has the same dimensions as the most recent output.
        !*/

        const tensor& get_output(
        ) const;
        /*!
            ensures
                - returns the output for the last tensor that was run through the network.
                  If nothing has been run through the network yet then returns an empty
                  tensor. 
        !*/

        tensor& get_gradient_input(
        );
        /*!
            ensures
                - returns the error gradient for this network.  That is, this is the error
                  gradient that this network will use to update itself when update() is
                  called.  Therefore, when performing back propagation, layers that sit on
                  top of this network layer write their back propagated error gradients
                  into get_gradient_input().  Or to put it another way, during back
                  propagation, layers take the contents of their get_gradient_input() and
                  back propagate it through themselves and store the results into their
                  subnetwork's get_gradient_input().

                  This means you should consider get_gradient_input() as an input to the
                  update() method.  
        !*/

        const tensor& get_final_data_gradient(
        ) const;
        /*!
            ensures
                - if update() has been called to back-propagate a gradient through this
                  network then you can call get_final_data_gradient() to obtain the last
                  gradient computed.  That is, this function returns the gradient of the
                  network with respect to its inputs.
        !*/

        template <typename solver_type>
        void update(
            const tensor& x, 
            sstack<solver_type> solvers,
            double step_size
        );
        /*!
            requires
                - forward(x) was called to forward propagate x though the network.
                  Moreover, this was the most recent call to forward() and x has not been
                  subsequently modified in any way.
                - get_gradient_input() has been set equal to the gradient of this network's
                  output with respect to some loss function.
                - The given solvers have only ever been used with this network.  That
                  is, if you want to call update() on some other neural network object then
                  you must NOT reuse the same solvers object.
                - solvers.size() >= num_layers
                - 0 < step_size <= 1
            ensures
                - Back propagates the error gradient, get_gradient_input(), through this
                  network and uses the provided solvers to update the network parameters.
                - The parameter delta vector output by the solvers is multiplied by
                  step_size before being added to the parameters.
                - All elements of #get_gradient_input() are set to 0. 
                - have_same_dimensions(#get_final_data_gradient(), x) == true
                - #get_final_data_gradient() contains the gradient of the network with
                  respect to x.
        !*/

        template <typename solver_type>
        void update(
            const tensor& x, 
            const tensor& gradient_input,
            sstack<solver_type> solvers,
            double step_size
        );
        /*!
            requires
                - forward(x) was called to forward propagate x though the network.
                  Moreover, this was the most recent call to forward() and x has not been
                  subsequently modified in any way.
                - have_same_dimensions(gradient_input, get_output()) == true
                - The given solvers have only ever been used with this network.  That
                  is, if you want to call update() on some other neural network object then
                  you must NOT reuse the same solvers object.
                - solvers.size() >= num_layers
                - 0 < step_size <= 1
            ensures
                - This function is identical to the version of update() defined immediately
                  above except that it back-propagates gradient_input through the network
                  instead of get_gradient_input().  Therefore, this version of update is
                  equivalent to performing:
                    get_gradient_input() = gradient_input;
                    update(x,solvers);
                  Except that calling update(x,gradient_input,solvers) avoids the copy
                  and is therefore slightly more efficient.
                - The parameter delta vector output by the solvers is multiplied by
                  step_size before being added to the parameters.
                - All elements of #get_gradient_input() are set to 0. 
                - #get_final_data_gradient() contains the gradient of the network with
                  respect to x.
        !*/

        void clean(
        );
        /*!
            ensures
                - Causes the network to forget about everything but its parameters.  
                  That is, for each layer we will have:
                    - get_output().num_samples() == 0
                    - get_gradient_input().num_samples() == 0
                  However, running new input data though this network will still produce
                  the same output it would have produced regardless of any calls to
                  clean().  The purpose of clean() is to compact the network object prior
                  to saving it to disk so that it takes up less space and the IO is
                  quicker.
        !*/

    };

    template <typename T, typename U>, 
    void serialize(const add_layer<T,U>& item, std::ostream& out);
    template <typename T, typename U>, 
    void deserialize(add_layer<T,U>& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    class no_label_type;

    template <
        typename LOSS_DETAILS, 
        typename SUBNET
        >
    class add_loss_layer
    {
        /*!
            REQUIREMENTS ON LOSS_DETAILS 
                - Must be a type that implements the EXAMPLE_LOSS_LAYER_ interface defined
                  in loss_abstract.h
                - LOSS_DETAILS::sample_expansion_factor == SUBNET::sample_expansion_factor
                  i.e. The loss layer and input layer must agree on the sample_expansion_factor.

            REQUIREMENTS ON SUBNET
                - One of the following must be true:
                    - SUBNET is an add_layer object.
                    - SUBNET is an add_tag_layer object.
                    - SUBNET is an add_skip_layer object.
                    - SUBNET is a repeat object.

            WHAT THIS OBJECT REPRESENTS
                This object represents a deep neural network.  In particular, it is a tool
                for adding a loss layer on top of the neural network of type SUBNET, which
                is specified as a template argument.  The specific layer added is defined
                by the LOSS_DETAILS details template argument.  Importantly, a loss layer
                is the last layer in a deep neural network.  So once it is added you can't
                add any other layers of any type.
        !*/

    public:
        typedef LOSS_DETAILS loss_details_type;
        typedef SUBNET subnet_type;
        typedef typename subnet_type::input_type input_type;
        // Note that the loss layer doesn't count as an additional layer since it doesn't
        // have any learnable parameters.  
        const static size_t num_layers = subnet_type::num_layers;
        const static unsigned int sample_expansion_factor = subnet_type::sample_expansion_factor;
        // If LOSS_DETAILS is an unsupervised loss then label_type==no_label_type.
        // Otherwise it is defined as follows:
        typedef typename LOSS_DETAILS::label_type label_type;



        add_loss_layer() = default;
        /*!
            ensures
                - default constructs all the layers in this network.
        !*/

        add_loss_layer(const add_loss_layer&) = default;
        add_loss_layer(add_loss_layer&&) = default;
        add_loss_layer& operator=(add_loss_layer&&) = default;
        add_loss_layer& operator=(const add_loss_layer&) = default;
        /*!
            ensures
                - this object is copyable and movable.
        !*/

        template <typename T, typename U>
        add_loss_layer(
            const add_loss_layer<T,U>& item
        );
        /*!
            ensures
                - This constructor allows you to copy neural network objects from one to
                  another as long as their corresponding layers can be constructed from
                  each other.
                - #loss_details() == loss_details_type(item.loss_details())
                - #subnet()       == subnet_type(item.subnet())
        !*/

        template <typename ...T>
        add_loss_layer(
            const LOSS_DETAILS& layer_det, 
            T&& ...args
        ); 
        /*!
            ensures
                - #loss_details() == layer_det
                - #subnet()       == subnet_type(args)
        !*/

        template <typename ...T>
        add_loss_layer(
            LOSS_DETAILS&& layer_det, 
            T&& ...args
        );
        /*!
            ensures
                - #loss_details() == layer_det
                - #subnet()       == subnet_type(args)
        !*/

        template <typename ...T>
        add_loss_layer(
            T ...args
        ); 
        /*!
            ensures
                - #loss_details() == loss_details_type()
                - #subnet()       == subnet_type(args)
        !*/

        const subnet_type& subnet(
        ) const; 
        /*!
            ensures
                - returns the immediate subnetwork of *this network.  
        !*/

        subnet_type& subnet(
        ); 
        /*!
            ensures
                - returns the immediate subnetwork of *this network.  
        !*/

        const loss_details_type& loss_details(
        ) const; 
        /*!
            ensures
                - returns the loss_details_type instance that defines the behavior of the
                  loss layer used by this network.
        !*/

        loss_details_type& loss_details(
        ); 
        /*!
            ensures
                - returns the loss_details_type instance that defines the behavior of the
                  loss layer used by this network.
        !*/

        template <typename input_iterator>
        void to_tensor (
            input_iterator ibegin,
            input_iterator iend,
            resizable_tensor& data
        ) const;
        /*!
            requires
                - [ibegin, iend) is an iterator range over input_type objects.
                - std::distance(ibegin,iend) > 0
            ensures
                - Converts the iterator range into a tensor and stores it into #data.
                - #data.num_samples() == distance(ibegin,iend)*sample_expansion_factor. 
                - The data in the ith sample of #data corresponds to the input_type object
                  *(ibegin+i/sample_expansion_factor).
                - Invokes data.async_copy_to_device() so that the data begins transferring
                  to the GPU device, if present.
                - This function is implemented by calling the to_tensor() routine defined
                  at the input layer of this network.  
        !*/

    // -------------

        template <typename output_iterator>
        void operator() (
            const tensor& x, 
            output_iterator obegin
        );
        /*!
            requires
                - x.num_samples()%sample_expansion_factor == 0
                - x.num_samples() > 0
                - obegin == iterator pointing to the start of a range of
                  x.num_samples()/sample_expansion_factor label_type elements.
            ensures
                - runs x through the network and writes the output to the range at obegin.
                - loss_details().to_label() is used to write the network output into
                  obegin.
        !*/

        template <typename input_iterator, typename label_iterator>
        void operator() (
            input_iterator ibegin,
            input_iterator iend,
            label_iterator obegin
        );
        /*!
            requires
                - [ibegin, iend) is an iterator range over input_type objects.
                - std::distance(ibegin,iend) > 0
                - obegin == iterator pointing to the start of a range of
                  std::distance(ibegin,iend) label_type elements.
            ensures
                - runs [ibegin,iend) through the network and writes the output to the range
                  at obegin.
                - loss_details().to_label() is used to write the network output into
                  obegin.
        !*/

    // -------------

        const label_type& operator() (
            const input_type& x
        );
        /*!
            ensures
                - runs a single object, x, through the network and returns the output.
                - loss_details().to_label() is used to convert the network output into a
                  label_type.
        !*/

        template <typename iterable_type>
        std::vector<label_type> operator() (
            const iterable_type& data,
            size_t batch_size = 128
        );
        /*!
            requires
                - batch_size > 0
                - data must have a .begin() and .end() that supply iterators over a
                  sequence of input_type elements.  E.g. data could have a type of
                  std::vector<input_type>
            ensures
                - runs all the objects in data through the network and returns their
                  predicted labels.  This means this function returns a vector V such that:
                    - V.size() == data.size()
                    - for all valid i: V[i] == the predicted label of data[i].
                - Elements of data are run through the network in batches of batch_size
                  items.  Using a batch_size > 1 can be faster because it better exploits
                  the available hardware parallelism.
                - loss_details().to_label() is used to convert the network output into a
                  label_type.
        !*/

    // -------------

        template <typename label_iterator>
        double compute_loss (
            const tensor& x,
            label_iterator lbegin 
        );
        /*!
            requires
                - x.num_samples()%sample_expansion_factor == 0
                - x.num_samples() > 0
                - lbegin == iterator pointing to the start of a range of
                  x.num_samples()/sample_expansion_factor label_type elements.
            ensures
                - runs x through the network, compares the output to the expected output
                  pointed to by lbegin, and returns the resulting loss. 
                - for all valid k:
                    - the expected label of the kth sample in x is *(lbegin+k/sample_expansion_factor).
                - This function does not update the network parameters.
        !*/

        template <typename input_iterator, typename label_iterator>
        double compute_loss (
            input_iterator ibegin,
            input_iterator iend,
            label_iterator lbegin 
        );
        /*!
            requires
                - [ibegin, iend) is an iterator range over input_type objects.
                - std::distance(ibegin,iend) > 0
                - lbegin == iterator pointing to the start of a range of
                  std::distance(ibegin,iend) label_type elements.
            ensures
                - runs [ibegin,iend) through the network, compares the output to the
                  expected output pointed to by lbegin, and returns the resulting loss. 
                - for all valid k:
                    - the expected label of *(ibegin+k) is *(lbegin+k).
                - This function does not update the network parameters.
        !*/

    // -------------

        double compute_loss (
            const tensor& x
        );
        /*!
            requires
                - LOSS_DETAILS is an unsupervised loss.  i.e. label_type==no_label_type.
                - x.num_samples()%sample_expansion_factor == 0
                - x.num_samples() > 0
            ensures
                - runs x through the network and returns the resulting loss. 
                - This function does not update the network parameters.
        !*/

        template <typename input_iterator>
        double compute_loss (
            input_iterator ibegin,
            input_iterator iend,
        );
        /*!
            requires
                - LOSS_DETAILS is an unsupervised loss.  i.e. label_type==no_label_type.
                - [ibegin, iend) is an iterator range over input_type objects.
                - std::distance(ibegin,iend) > 0
            ensures
                - runs [ibegin,iend) through the network and returns the resulting loss. 
                - This function does not update the network parameters.
        !*/

    // -------------

        template <typename label_iterator, typename solver_type>
        double update (
            const tensor& x,
            label_iterator lbegin,
            sstack<solver_type> solvers,
            double step_size
        );
        /*!
            requires
                - x.num_samples()%sample_expansion_factor == 0
                - x.num_samples() > 0
                - lbegin == iterator pointing to the start of a range of
                  x.num_samples()/sample_expansion_factor label_type elements.
                - The given solvers have only ever been used with this network.  That
                  is, if you want to call update() on some other neural network object then
                  you must NOT reuse the same solvers object.
                - solvers.size() >= num_layers
                - 0 < step_size <= 1
            ensures
                - runs x through the network, compares the output to the expected output
                  pointed to by lbegin, and updates the network parameters via
                  backpropagation.
                - for all valid k:
                    - the expected label of the kth sample in x is *(lbegin+k/sample_expansion_factor).
                - The provided solvers are used to update the parameters in each layer of
                  the network.
                - The parameter delta vector output by the solvers is multiplied by
                  step_size before being added to the parameters.
                - returns compute_loss(x,lbegin)
        !*/

        template <typename input_iterator, typename label_iterator, typename solver_type>
        double update (
            input_iterator ibegin,
            input_iterator iend,
            label_iterator lbegin,
            sstack<solver_type> solvers,
            double step_size
        );
        /*!
            requires
                - [ibegin, iend) is an iterator range over input_type objects.
                - std::distance(ibegin,iend) > 0
                - lbegin == iterator pointing to the start of a range of
                  std::distance(ibegin,iend) label_type elements.
                - The given solvers have only ever been used with this network.  That
                  is, if you want to call update() on some other neural network object then
                  you must NOT reuse the same solvers object.
                - solvers.size() >= num_layers
                - 0 < step_size <= 1
            ensures
                - runs [ibegin,iend) through the network, compares the output to the
                  expected output pointed to by lbegin, and updates the network parameters
                  via backpropagation.
                - for all valid k:
                    - the expected label of *(ibegin+k) is *(lbegin+k).
                - The provided solvers are used to update the parameters in each layer of
                  the network.
                - The parameter delta vector output by the solvers is multiplied by
                  step_size before being added to the parameters.
                - returns compute_loss(ibegin,iend,lbegin)
        !*/

    // -------------

        template <typename solver_type>
        double update (
            const tensor& x,
            sstack<solver_type> solvers,
            double step_size
        );
        /*!
            requires
                - LOSS_DETAILS is an unsupervised loss.  i.e. label_type==no_label_type.
                - x.num_samples()%sample_expansion_factor == 0
                - x.num_samples() > 0
                - The given solvers have only ever been used with this network.  That
                  is, if you want to call update() on some other neural network object then
                  you must NOT reuse the same solvers object.
                - solvers.size() >= num_layers
                - 0 < step_size <= 1
            ensures
                - runs x through the network and updates the network parameters by
                  back-propagating the loss gradient through the network.
                - The provided solvers are used to update the parameters in each layer of
                  the network.
                - The parameter delta vector output by the solvers is multiplied by
                  step_size before being added to the parameters.
                - returns compute_loss(x)
        !*/

        template <typename input_iterator, typename solver_type>
        double update (
            input_iterator ibegin,
            input_iterator iend,
            sstack<solver_type> solvers,
            double step_size
        );
        /*!
            requires
                - LOSS_DETAILS is an unsupervised loss.  i.e. label_type==no_label_type.
                - [ibegin, iend) is an iterator range over input_type objects.
                - std::distance(ibegin,iend) > 0
                - The given solvers have only ever been used with this network.  That
                  is, if you want to call update() on some other neural network object then
                  you must NOT reuse the same solvers object.
                - solvers.size() >= num_layers
                - 0 < step_size <= 1
            ensures
                - runs [ibegin,iend) through the network and updates the network parameters
                  by back-propagating the loss gradient through the network.
                - The provided solvers are used to update the parameters in each layer of
                  the network.
                - The parameter delta vector output by the solvers is multiplied by
                  step_size before being added to the parameters.
                - returns compute_loss(ibegin,iend)
        !*/

    // -------------

        void clean (
        );
        /*!
            ensures
                - Causes the network to forget about everything but its parameters.  
                - invokes subnet().clean()
        !*/
    };

    template <typename T, typename U>, 
    void serialize(const add_loss_layer<T,U>& item, std::ostream& out);
    template <typename T, typename U>, 
    void deserialize(add_loss_layer<T,U>& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    template <typename ...T>
    decorator_repeat_group<T...> repeat_group (
        T&& ...args
    );
    /*!
        ensures
            - Decorates a group of variables.  This is essentially like std::make_tuple()
              except it's only purpose is to group variables together so they can be passed
              to the repeat object's constructor.
    !*/

    template <
        size_t num,
        template<typename> class REPEATED_LAYER, 
        typename SUBNET
        >
    class repeat 
    {
        /*!
            REQUIREMENTS ON num
                - num > 0

            REQUIREMENTS ON REPEATED_LAYER
                - REPEATED_LAYER must be a template that stacks more layers onto a deep neural
                  network.  For example, if net_type were a network without a loss layer,
                  then it should be legal to create a deeper network with a type of
                  REPEATED_LAYER<net_type>.

            REQUIREMENTS ON SUBNET
                - One of the following must be true:
                    - SUBNET is an add_layer object.
                    - SUBNET is an add_tag_layer object.
                    - SUBNET is an add_skip_layer object.
                    - SUBNET is a repeat object.

            WHAT THIS OBJECT REPRESENTS
                This object adds more layers to a deep neural network.  In particular, it
                adds REPEATED_LAYER on top of SUBNET num times.  So for example, if num were 2 then
                repeat<2,REPEATED_LAYER,SUBNET> would create a network equivalent to REPEATED_LAYER<REPEATED_LAYER<SUBNET>>.

                Also, this object provides an interface identical to the one defined by the
                add_layer object except that we add the num_repetitions() and
                get_repeated_layer() methods.  These additions are shown below along with
                some additional explanatory comments.
        !*/

    public:

        typedef SUBNET subnet_type;
        typedef typename SUBNET::input_type input_type;
        const static size_t num_layers = (REPEATED_LAYER<SUBNET>::num_layers-SUBNET::num_layers)*num + SUBNET::num_layers;
        const static unsigned int sample_expansion_factor = SUBNET::sample_expansion_factor;
        typedef REPEATED_LAYER<an_unspecified_input_type> repeated_layer_type;

        template <typename T, typename ...U>
        repeat(
            T arg1,
            U ...args2
        );
        /*!
            ensures
                - arg1 is used to initialize the num_repetitions() copies of REPEATED_LAYER inside
                  this object.  That is, all the REPEATED_LAYER elements are initialized identically
                  by being given copies of arg1.
                - The rest of the arguments to the constructor, i.e. args2, are passed to
                  SUBNET's constructor.  
        !*/

        template <typename ...T, typename ...U>
        repeat(
            decorator_repeat_group<T...>&& arg1,
            U ...args2
        );
        /*!
            ensures
                - arg1 is used to initialize the num_repetitions() copies of REPEATED_LAYER inside
                  this object.  That is, all the REPEATED_LAYER elements are initialized identically
                  by being given copies of an undecorated arg1.
                - The rest of the arguments to the constructor, i.e. args2, are passed to
                  SUBNET's constructor.  
        !*/

        size_t num_repetitions (
        ) const; 
        /*!
            ensures
                - returns num (i.e. the number of times REPEATED_LAYER was stacked on top of SUBNET)
        !*/

        const repeated_layer_type& get_repeated_layer (
            size_t i 
        ) const;
        /*!
            requires
                - i < num_repetitions()
            ensures
                - returns a reference to the i-th instance of REPEATED_LAYER.  For example,
                  get_repeated_layer(0) returns the instance of REPEATED_LAYER that is on the top of
                  the network while get_repeated_layer(num_repetitions()-1) returns the
                  instance of REPEATED_LAYER that is stacked immediately on top of SUBNET.
        !*/

        repeated_layer_type& get_repeated_layer (
            size_t i 
        );
        /*!
            requires
                - i < num_repetitions()
            ensures
                - returns a reference to the i-th instance of REPEATED_LAYER.  For example,
                  get_repeated_layer(0) returns the instance of REPEATED_LAYER that is on the top of
                  the network while get_repeated_layer(num_repetitions()-1) returns the
                  instance of REPEATED_LAYER that is stacked immediately on top of SUBNET.
        !*/

        const subnet_type& subnet(
        ) const; 
        /*!
            ensures
                - returns the SUBNET base network that repeat sits on top of.  If you want
                  to access the REPEATED_LAYER components then you must use get_repeated_layer(). 
        !*/

        subnet_type& subnet(
        ); 
        /*!
            ensures
                - returns the SUBNET base network that repeat sits on top of.  If you want
                  to access the REPEATED_LAYER components then you must use get_repeated_layer(). 
        !*/
    };

// ----------------------------------------------------------------------------------------

    template <
        unsigned long ID, 
        typename SUBNET
        >
    class add_tag_layer
    {
        /*!
            REQUIREMENTS ON SUBNET
                - One of the following must be true:
                    - SUBNET implements the EXAMPLE_INPUT_LAYER interface defined in
                      input_abstract.h.
                    - SUBNET is an add_layer object.
                    - SUBNET is an add_tag_layer object.
                    - SUBNET is an add_skip_layer object.
                    - SUBNET is a repeat object.

            WHAT THIS OBJECT REPRESENTS
                This object adds a new layer to a deep neural network.  However, this layer
                simply performs the identity transform.  This means it is a no-op and its
                presence does not change the behavior of the network.  It exists solely to
                be used by add_skip_layer to reference a particular part of a network.

                Also, this object provides an interface identical to the one defined by the
                add_layer object.
        !*/
    };

    template <unsigned long ID, typename U>, 
    void serialize(const add_tag_layer<ID,U>& item, std::ostream& out);
    template <unsigned long ID, typename U>, 
    void deserialize(add_tag_layer<ID,U>& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET> using tag1  = add_tag_layer< 1, SUBNET>;
    template <typename SUBNET> using tag2  = add_tag_layer< 2, SUBNET>;
    template <typename SUBNET> using tag3  = add_tag_layer< 3, SUBNET>;
    template <typename SUBNET> using tag4  = add_tag_layer< 4, SUBNET>;
    template <typename SUBNET> using tag5  = add_tag_layer< 5, SUBNET>;
    template <typename SUBNET> using tag6  = add_tag_layer< 6, SUBNET>;
    template <typename SUBNET> using tag7  = add_tag_layer< 7, SUBNET>;
    template <typename SUBNET> using tag8  = add_tag_layer< 8, SUBNET>;
    template <typename SUBNET> using tag9  = add_tag_layer< 9, SUBNET>;
    template <typename SUBNET> using tag10 = add_tag_layer<10, SUBNET>;

// ----------------------------------------------------------------------------------------

    template <
        template<typename> class TAG_TYPE, 
        typename SUBNET
        >
    class add_skip_layer
    {
        /*!
            REQUIREMENTS ON SUBNET
                - One of the following must be true:
                    - SUBNET is an add_layer object.
                    - SUBNET is an add_tag_layer object.
                    - SUBNET is an add_skip_layer object.
                    - SUBNET is a repeat object.

            WHAT THIS OBJECT REPRESENTS
                This object adds a new layer to a deep neural network which draws its
                inputs from layer<TAG_TYPE>(subnet()) and performs the identity transform.

                Also, this object provides an interface identical to the one defined by the
                add_layer object.
        !*/
    };

    template <template<typename> class T, typename U>
    void serialize(const add_skip_layer<T,U>& item, std::ostream& out);
    template <template<typename> class T, typename U>
    void deserialize(add_skip_layer<T,U>& item, std::istream& in);
    /*!
        provides serialization support  
    !*/

    template <typename SUBNET> using skip1  = add_skip_layer< tag1, SUBNET>;
    template <typename SUBNET> using skip2  = add_skip_layer< tag2, SUBNET>;
    template <typename SUBNET> using skip3  = add_skip_layer< tag3, SUBNET>;
    template <typename SUBNET> using skip4  = add_skip_layer< tag4, SUBNET>;
    template <typename SUBNET> using skip5  = add_skip_layer< tag5, SUBNET>;
    template <typename SUBNET> using skip6  = add_skip_layer< tag6, SUBNET>;
    template <typename SUBNET> using skip7  = add_skip_layer< tag7, SUBNET>;
    template <typename SUBNET> using skip8  = add_skip_layer< tag8, SUBNET>;
    template <typename SUBNET> using skip9  = add_skip_layer< tag9, SUBNET>;
    template <typename SUBNET> using skip10 = add_skip_layer<tag10, SUBNET>;

// ----------------------------------------------------------------------------------------

    template <
        unsigned int i, 
        typename net_type
        >
    auto& layer (
        net_type& n
    );
    /*!
        requires
            - net_type is an object of type add_layer, add_loss_layer, add_skip_layer, or
              add_tag_layer.
        ensures
            - This function chains together i calls to n.subnet() and returns the
              result.  So for example:
                - if (i == 0)
                    - returns n
                - else if (i == 1)
                    - returns n.subnet()
                - else if (i == 2)
                    - returns n.subnet().subnet()
                - else if (i == 3)
                    - returns n.subnet().subnet().subnet()
                - else
                    - etc.
    !*/

    template <
        template<typename> class Match, 
        typename net_type 
        >
    auto& layer (
        net_type& n
    );
    /*!
        requires
            - net_type is an object of type add_layer, add_loss_layer, add_skip_layer, or
              add_tag_layer.
        ensures
            - returns the first layer in n that is of type Match.  E.g. if net_type is
              fc<relu<fc<input<sample_type>>>> then calling layer<relu>(n) would return
              layer<1>(n), that is, a reference to the relu layer.
    !*/

    template <
        template<typename> class Match, 
        unsigned int i, 
        typename net_type
        >
    auto& layer (
        net_type& n
    );
    /*!
        requires
            - net_type is an object of type add_layer, add_loss_layer, add_skip_layer, or
              add_tag_layer.
        ensures
            - returns layer<i>(layer<Match>(n))
    !*/

// ----------------------------------------------------------------------------------------

    struct layer_test_results
    {
        std::string log;
        bool was_good;

        operator bool() const { return was_good; }
    };

    inline std::ostream& operator<< (std::ostream& out, const layer_test_results& item)
    {
        out << item.log;
        return out;
    }

    template <
        typename layer_details_type
        >
    layer_test_results test_layer (
        layer_details_type l
    );
    /*!
        ensures
            - Checks if l correctly implements the EXAMPLE_LAYER_ interface defined in
              layers_abstract.h.  Importantly, it computes numerical approximations to the
              gradients and compares them to the outputs of the layer.  
            - The results of the testing are returned.  In particular, if the returned object
              is RESULT then we will have:
                - RESULT.was_good == false if and only if the layer failed the testing.
                - RESULT.log == a string describing why the testing failed if was_good==false.
            - Note that this function is only capable of checking layers that take
              arbitrary subnetworks as input.  So if you have designed a layer that expects
              only a certain restricted type of subnetwork then you might get a compile or
              runtime error when you call this function.
    !*/

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_DNn_CORE_ABSTRACT_H_ 

