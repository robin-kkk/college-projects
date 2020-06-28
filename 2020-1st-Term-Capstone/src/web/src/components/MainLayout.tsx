import React, { useState, useEffect } from "react";
import { Layout, Menu } from "antd";

import { Link, withRouter, RouteProps } from "react-router-dom";

const { Header, Content, Footer } = Layout;

type MainLayoutProps = React.HTMLProps<HTMLDivElement> & RouteProps;

const MainLayout = (props: MainLayoutProps) => {
  const [currentKey, setCurrentKey] = useState("info");
  const [isShownHeader, setIsShownHeader] = useState(true);

  useEffect(() => {
    const pathname = window.location.pathname.replace("/", "");
    setCurrentKey(pathname === "" ? "info" : pathname);
    if (pathname.includes("detail")) {
      setIsShownHeader(false);
    }
  }, [props.location]);

  return (
    <Layout style={{ minHeight: "100%" }}>
      {isShownHeader && (
        <Header>
          <Menu mode="horizontal" theme="dark" selectedKeys={[currentKey]}>
            <Menu.Item key="info">
              <Link to="/">Home</Link>
            </Menu.Item>
            <Menu.Item key="about">
              <Link to="/about">About</Link>
            </Menu.Item>
            <Menu.Item key="term">
              <Link to="/term">Term</Link>
            </Menu.Item>
            <Menu.Item key="tag">
              <Link to="/tag">Tag</Link>
            </Menu.Item>
            <Menu.Item key="goldencross">
              <Link to="/golden-cross">Golden Cross</Link>
            </Menu.Item>
          </Menu>
        </Header>
      )}
      <Content style={{ padding: "30px" }}>{props.children}</Content>
      {isShownHeader && <Footer>Capstone Project</Footer>}
    </Layout>
  );
};

export default withRouter(MainLayout);
