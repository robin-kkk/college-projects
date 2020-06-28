import React, { useEffect, useState, useCallback } from "react";

import StockItemList from "~/components/StockItemList";
import { PageHeader, Table } from "antd";
import StockItem, { makeIndexItem } from "~/utils/interfaces/StockItem";
import TableColumns from "~/utils/functions/TableColumns";
import MainService from "~/services/MainService";

const HomePage = () => {
  const service = new MainService();

  const [reHottest, setReHottest] = useState<StockItem[]>([]);
  const [reRandom, setReRandom] = useState<StockItem[]>([]);

  const [hottest, setHottest] = useState<StockItem[]>([]);

  useEffect(() => {
    const callingService = async () => {
      const ho = await service.getHottest();

      const reHo = await service.recommendByHottest();
      const reRa = await service.recommendByRandom();

      setReHottest(makeIndexItem(reHo.content));
      setReRandom(makeIndexItem(reRa.content));

      setHottest(makeIndexItem(ho.content.slice(0, 30)));
    };

    callingService();
  }, []);

  const handleRefrestHottest = useCallback(async () => {
    setReHottest([]);
    const reHo = await service.recommendByHottest();
    setReHottest(makeIndexItem(reHo.content));
  }, []);

  const handleRefreshRnadom = useCallback(async () => {
    setReRandom([]);
    const reRa = await service.recommendByRandom();
    setReRandom(reRa.content);
  }, []);

  return (
    <>
      <StockItemList
        title="떠오르는 종목"
        stocks={reHottest}
        onRefresh={handleRefrestHottest}
      />
      <StockItemList
        title="오늘의 추천"
        stocks={reRandom}
        style={{ marginTop: "20px" }}
        onRefresh={handleRefreshRnadom}
      />
      <div
        style={{ backgroundColor: "white", padding: "20px", marginTop: "20px" }}
      >
        <PageHeader title="인기종목 TOP 30" />
        <Table dataSource={hottest} columns={TableColumns} />
      </div>
    </>
  );
};

export default HomePage;
