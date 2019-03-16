## Component

This package contains the component pattern that I use a lot when building ReasonReact Component.

## What's available in this library

- [x] `Component`, a functor to create JSX by only specifying the state

## Component

```ocaml

open ReComponent;

module Counter = Component.Make({
    type t = int
})


/*
  inside your render
*/


<Counter intialState=0>
  ...{({setState, state}) => {
      <>
        <p>
            {ReasonReact.string(state->string_of_int)}
        </p>
        <button onClick={_ => {
            setState(state + 1);
        }}>
            <p>
                {ReasonReact.string("increment")}
            </p>
        </button>
      </>
  }}
</Counter>

```

## Inspiration

- [Component by Ryan Florence](https://github.com/reactions/component)
- [recontainers by Astrocoders](https://github.com/Astrocoders/recontainers)

## Have question about this

[ask me on twitter](https://twitter.com/broerjuang)
