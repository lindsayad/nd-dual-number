# Motivation and Background

For the Multiphysics Object-Oriented Simulation Environment (MOOSE) we wanted to
add automatic differentiation (AD) capabilities in order to ensure more
efficient numerical analysis. @roystgnr had developed the software package
[MetaPhysicL](https://github.com/roystgnr/metaphysicl) which enabled AD
calculations with scalar types (`float`, `double`, etc.) using the class
template `DualNumber`. However, in MOOSE we had a `MaterialProperty` class
template that worked on non-scalar types. So I needed to add a more general AD
structure to `MetaPhysicL` that would:

1. allow automatic differentiation calculations for non-scalar types
2. allow storage of a single object that could be queried for:
    1. a raw value (without derivative information)
    2. a "dual" value that contains both value and derivative information

If we imagine a template class:

```c++
template <typename T>
class Vector;
```

one could already accomplish requirement 1) with `MetaPhysicL` by instantiating
a `Vector<DualNumber<double>>`. However, for MOOSE purposes we would have to
store two objects, a `Vector<double>` and `Vector<DualNumber<double>>` and
perform synchronizations between the two objects throughout a simulation. These
synchronizations were at the time pratically prohibitive, so I developed the
`NotADuckDualNumber` class template which is included in this repository
(defined in `nddualnumber.h`) which simultaneously satisfies both criteria 1) and
2).

# Repo Contents Summary

- `NotADuckDualNumber` dependencies (developed by @roystgnr)
    - `dualderivatives.h`
    - `ct_types.h`
    - `numberarray.h`
    - `raw_type.h`
    - `compare_types.h`
    - `dualnumber.h`
    - `dualnumber_decl.h`
    - `testable.h`
- `DualNumberSurrogate`: a mediating class between `NotADuckDualNumber` and
  `DualNumber` instantiations (@lindsayad)
    - `dualnumber_surrogate_decl.h`: `DualNumberSurrogate` class definition
    - `dualnumber_surrogate.h`: `DualNumberSurrogate` method definitions
- `NotADuckDualNumber`: the central class of this repo (@lindsayad)
    - `nddualnumber.h`: class amnd method definitions for `NotADuckDualNumber`
- A test that instantiates "dual" versions of tensors and vectors and ensures
  that derivatives are propagated through common tensor/vector operations like
  multiplication, addition, and norms. This test additionally confirms that
  operations on `NotADuckDualNumber<Tensor<double>>` and
  `Tensor<DualNumber<double>>` (or a `Vector` analog) yield identical results
  (@lindsayad)
    - `math_structs.h`: template definitions for `Tensor`, `Vector`,
      `NotADuckDualNumber<Tensor>`, and `NotADuckDualNumber<Vector>`
    - `nd_derivs_unit.C`: the source file that actually exercises the test

# Compilation

You can compile the test by simply running:
```
$ clang++ -std=c++11 -I. nd_derivs_unit.C
```
or with `g++`.
