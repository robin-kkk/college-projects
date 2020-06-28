import React from "react";
import { Route, Switch, BrowserRouter as Router } from "react-router-dom";

import "antd/dist/antd.css";

import Home from "~/contexts/Home";
import MainLayout from "~/components/MainLayout";
import TermPage from "~/contexts/Term";
import TagPage from "~/contexts/Tag";
import AboutPage from "~/contexts/About";
import GoldenCrossPage from "~/contexts/GoldenCross";
import DetailPage from "./contexts/Detail";

const App = () => (
  <Router>
    <MainLayout>
      <Switch>
        <Route path="/term" exact>
          <TermPage />
        </Route>
        <Route path="/tag" exact>
          <TagPage />
        </Route>
        <Route path="/about" exact>
          <AboutPage />
        </Route>
        <Route path="/golden-cross" exact>
          <GoldenCrossPage />
        </Route>
        <Route path="/detail/:code" exact>
          <DetailPage />
        </Route>
        <Route path="/" exact>
          <Home />
        </Route>
      </Switch>
    </MainLayout>
  </Router>
);

export default App;
