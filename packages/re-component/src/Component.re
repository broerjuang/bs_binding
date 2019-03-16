module type Config = {type t;};

module Make = (C: Config) => {
  type state = C.t;
  type action =
    | SetState(C.t);

  type render = {
    setState: C.t => unit,
    state: C.t,
  };

  let component = ReasonReact.reducerComponent("Component");

  let make =
      (
        ~initialState,
        ~didMount=ignore,
        ~willUpdate=ignore,
        ~willUnmount=ignore,
        children,
      ) => {
    ...component,
    initialState,
    didMount,
    willUpdate,
    willUnmount,
    reducer: (action, _state) => {
      switch (action) {
      | SetState(newState) => ReasonReact.Update(newState)
      };
    },
    render: self =>
      children({
        setState: newState => self.send(SetState(newState)),
        state: self.state,
      }),
  };
};