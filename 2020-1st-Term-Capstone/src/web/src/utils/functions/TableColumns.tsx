import React from "react";
import { ColumnsType } from "antd/lib/table";

import StockItem, { makeInfoPageUrl } from "../interfaces/StockItem";
import { ArrowUpOutlined, ArrowDownOutlined } from "@ant-design/icons";

const TableColumns: ColumnsType<StockItem> = [
  {
    title: "순위",
    dataIndex: "index",
    key: "index",
    render: (index) => `${index + 1}위`,
  },
  {
    title: "종목명",
    dataIndex: "name",
    key: "name",
    render: (name: string, record: StockItem) => (
      <a href={makeInfoPageUrl(record.code)} target="_blank">
        {name}({record.code})
      </a>
    ),
  },
  {
    title: "현재가",
    dataIndex: "now_val",
    key: "now_val",
    render: (now_val: number, record: StockItem) => (
      <>
        {now_val?.toLocaleString()}원
        <span style={{ color: record.fluctuation > 0 ? "red" : "blue" }}>
          (
          {record.fluctuation > 0 ? <ArrowUpOutlined /> : <ArrowDownOutlined />}
          {record.fluctuation}%)
        </span>
      </>
    ),
  },
  {
    title: "거래량",
    dataIndex: "volume",
    key: "volume",
    render: (volume: number) => `${volume?.toLocaleString()}주`,
  },
];

export default TableColumns;
