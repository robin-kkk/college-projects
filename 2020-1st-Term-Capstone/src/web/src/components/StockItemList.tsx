import React, { HTMLProps } from "react";

import StockItem from "~/utils/interfaces/StockItem";
import {
  PageHeader,
  Card,
  Col,
  Row,
  Descriptions,
  Button,
  Tooltip,
} from "antd";
import {
  ArrowUpOutlined,
  ArrowDownOutlined,
  RetweetOutlined,
  LoadingOutlined,
} from "@ant-design/icons";
import { Link } from "react-router-dom";

interface StockItemListProps extends HTMLProps<HTMLDivElement> {
  stocks: StockItem[];
  maxDisplayItems?: number;
  onRefresh?: () => void;
}

const StockItemList = (props: StockItemListProps) => {
  const {
    stocks,
    maxDisplayItems = 45,
    title,
    onRefresh,
    style,
    ...restProps
  } = props;

  return (
    <div
      style={{ backgroundColor: "white", padding: "20px", ...style }}
      {...restProps}
    >
      <PageHeader
        title={title}
        extra={[
          onRefresh ? (
            <Tooltip placement="top" title="새로고침">
              <Button size="large" type="primary" onClick={onRefresh}>
                <RetweetOutlined />
              </Button>
            </Tooltip>
          ) : undefined,
        ].filter((e) => e)}
      />
      {stocks.length ? (
        Array.from({ length: Math.ceil(stocks.length / 5) }).map((_, index) => (
          <Row gutter={[5, 5]} key={`Row${index}`}>
            {[
              ...stocks.slice(index * 5, (index + 1) * 5),
              ...[undefined, undefined, undefined, undefined],
            ]
              .slice(0, 5)
              .map((stock, stockIndex) =>
                stock ? (
                  <Col flex="1 1 0" key={stock.code}>
                    <Link to={`/detail/${stock.code}`} target="_blank">
                      <Card size="small" title={`${stock.name}(${stock.code})`}>
                        <div
                          style={{
                            textAlign: "right",
                          }}
                        >
                          {stock.now_val?.toLocaleString()}원
                          <span
                            style={{
                              color: stock.fluctuation > 0 ? "red" : "blue",
                            }}
                          >
                            (
                            {stock.fluctuation > 0 ? (
                              <ArrowUpOutlined />
                            ) : (
                              <ArrowDownOutlined />
                            )}
                            {stock.fluctuation}%)
                          </span>
                          <br />
                          거래량: {stock.volume?.toLocaleString()}주
                        </div>
                      </Card>
                    </Link>
                  </Col>
                ) : (
                  <Col flex="1 1 0" key={stockIndex}></Col>
                ),
              )}
          </Row>
        ))
      ) : (
        <div style={{ textAlign: "center" }}>
          <LoadingOutlined style={{ marginTop: "30px", fontSize: "50px" }} />
        </div>
      )}
    </div>
  );
};

export default StockItemList;
