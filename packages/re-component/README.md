## Component

This package contains the component pattern that I use a lot when building ReasonReact Component.

## What's available in this library

- [x] `Component`, a functor to create JSX by only specifying the state
- [x] `Remote`, talking with remote server using; the assumption is the consumer using `Rest API` as `http communication`

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

## Remote

### Query

```ocaml

open ReComponent.Remote;

module GetTodos = Query.Make({
    (*
      Todo.t is coming from your data model. It's better to keep it separated from this, since you might using it not only when talking with remote server, but also when mapping value to your ui
    *)
    type response = list(Todo.t)
})


/*
  inside your render function

  responseDecoder is a function that accept Js.Json.t => response.
  if you don't want to write decoder and encoder manually. I suggest you to use atdgen instead. Please read this awesome blogpost:
  https://tech.ahrefs.com/getting-started-with-atdgen-and-bucklescript-1f3a14004081
*/


<GetTodos url="http://localhost:3000/todos" responseDecoder>
  ...{(result, refetch, data) => {
    switch (result) {
      | Idle => switch(data) {
        | Some(todos) => <TodosComponent todos>
        | None => <Placeholder>
      }
      | Loading => <LoadingSpinner />
      | Error(message) => <Error message refetch>
      | Data(todos) => <TodosComponent todos>
    }
  }}
</Counter>

```

## Inspiration

- [Component by Ryan Florence](https://github.com/reactions/component)
- [recontainers by Astrocoders](https://github.com/Astrocoders/recontainers)
- [reason apollo](https://github.com/apollographql/reason-apollo)

## Have question about this

[ask me on twitter](https://twitter.com/broerjuang)
