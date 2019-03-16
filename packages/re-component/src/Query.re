module type Config = {type response;};

module Make = (C: Config) => {
  /*
       The return type is using the variant type like ReasonReact Apollo
       https://github.com/apollographql/reason-apollo
   */

  type result =
    | Idle
    | Data(C.response)
    | Error(string)
    | Loading;

  /*
     Normalize the return type by defining refetch, so it's up to the consumer of the component
     wheter they want to listen when the result has changed
   */
  type render = {
    result,
    refetch: unit => unit,
    data: option(C.response),
  };

  type state = {
    result,
    data: option(C.response),
  };

  type action =
    | FetchRequested
    | FetchFailed(string)
    | FetchSucceed(C.response);

  let component = ReasonReact.reducerComponent("Query_ReComponent");

  let make =
      (
        ~url,
        ~responseDecoder,
        ~query=?,
        ~didMount=?,
        ~willUpdate=ignore,
        ~willUnmount=ignore,
        children,
      ) => {
    ...component,
    initialState: () => {result: Idle, data: None},
    didMount: self => {
      /*
         The basic assumption is that when user is not really specified
         the didMount function, it will directly trigger fetch to the given url
       */
      switch (didMount) {
      | Some(fn) => fn(self)
      | None => self.send(FetchRequested)
      };
    },
    willUnmount,
    willUpdate,
    reducer: (action, state) => {
      switch (action) {
      | FetchRequested =>
        ReasonReact.UpdateWithSideEffects(
          {...state, result: Loading},
          self =>
            Js.Promise.
              /*
               Currently use Js.Promise, in the future I will try to use Future from https://github.com/RationalJS/future instead
               waiting until aborting promise is landed
               */
              (
                Fetch.fetch(url ++ Belt.Option.getWithDefault(query, ""))
                |> then_(Fetch.Response.json)
                |> then_(result => {
                     let result = responseDecoder(result);
                     self.send(FetchSucceed(result)) |> resolve;
                   })
                |> catch(_error =>
                     self.send(FetchFailed("Failed to query " ++ url))
                     |> resolve
                   )
                |> ignore
              ),
        )
      | FetchFailed(message) =>
        ReasonReact.Update({...state, result: Error(message)})
      | FetchSucceed(newData) =>
        ReasonReact.Update({result: Data(newData), data: newData->Some})
      };
    },
    render: self => {
      children({
        result: self.state.result,
        refetch: () => self.send(FetchRequested),
        data: self.state.data,
      });
    },
  };
};