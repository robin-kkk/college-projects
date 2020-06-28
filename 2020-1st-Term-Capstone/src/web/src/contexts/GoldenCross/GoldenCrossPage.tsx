import React, { useState, useEffect } from "react";
import { PageHeader, Table } from "antd";

import StockItem, { makeIndexItem } from "~/utils/interfaces/StockItem";
import TableColumns from "~/utils/functions/TableColumns";
import MainService from "~/services/MainService";

const GoldenCrossPage = () => {
  const service = new MainService();

  const [goldencross, setGoldencross] = useState<StockItem[]>([]);

  useEffect(() => {
    const callingService = async () => {
      const go = await service.getGoldencross();
      setGoldencross(makeIndexItem(go.content));
    };
    callingService();
  }, []);

  return (
    <div
      style={{ backgroundColor: "white", padding: "20px", marginTop: "20px" }}
    >
      <PageHeader title="골든크로스 TOP 15" />
      <Table dataSource={goldencross} columns={TableColumns} />
    </div>
  );
};

export default GoldenCrossPage;
