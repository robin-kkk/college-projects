import React, { useState, useMemo, useCallback } from "react";
import { Table, Input } from "antd";
import { SearchOutlined } from "@ant-design/icons";

import TermData from "~/static/Term";

const TermPage = () => {
  const [keyword, setKeyword] = useState("");

  const source = useMemo(() => {
    const dataWithKey = TermData.map(t => ({ ...t, key: t.name }));
    if (keyword) {
      return dataWithKey.filter(d => d.name.includes(keyword));
    }
    return dataWithKey;
  }, [keyword]);

  const handleChange = useCallback(e => {
    setKeyword(e.target.value);
  }, []);

  return (
    <>
      <div style={{ textAlign: "right" }}>
        <Input
          size="large"
          placeholder="검색할 용어"
          prefix={<SearchOutlined />}
          style={{ width: 300 }}
          onChange={handleChange}
        />
      </div>
      <Table
        size="large"
        bordered
        dataSource={source}
        columns={columns}
        pagination={false}
        expandable={{
          expandedRowRender: record => (
            <p style={{ margin: 0, whiteSpace: "pre-line" }}>
              {record.description}
            </p>
          ),
        }}
        style={{ marginTop: "20px" }}
      />
    </>
  );
};

export default TermPage;

const columns = [
  {
    title: "Name",
    dataIndex: "name",
    key: "name",
  },
];
