import React, { useEffect, useState } from "react";
import { withRouter, RouteComponentProps, Link } from "react-router-dom";
import { Row, Col, Card, PageHeader } from "antd";
import MainService from "~/services/MainService";
import StockItem from "~/utils/interfaces/StockItem";
import {
  ArrowUpOutlined,
  ArrowDownOutlined,
  LoadingOutlined,
} from "@ant-design/icons";

interface DetailParamsProps {
  code: string;
}

const DetailPage = ({ match }: RouteComponentProps<DetailParamsProps>) => {
  if (!match?.params?.code) {
    return null;
  }

  const service = new MainService();

  const [code, setCode] = useState("");
  const [items, setItems] = useState<StockItem[]>([]);
  const [isLoading, setIsLoading] = useState(false);

  useEffect(() => {
    setCode(match.params.code);

    const callingService = async () => {
      setIsLoading(true);
      console.log("A");
      const result = await service.recommendByItem(match.params.code);
      setItems(result.content);
      console.log("B");
      setIsLoading(false);
    };

    callingService();
  }, []);

  return (
    <Row gutter={[10, 10]}>
      <Col flex="250px">
        <PageHeader title="이런 주식은 어떠세요?" />
        {!isLoading ? (
          items.slice(0, 5).map((stock) => (
            <Col key={stock.code}>
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
          ))
        ) : (
          <div style={{ textAlign: "center" }}>
            <LoadingOutlined style={{ marginTop: "30px", fontSize: "50px" }} />
          </div>
        )}
      </Col>
      <Col flex="auto" style={{ textAlign: "center" }}>
        <iframe
          src={`https://navercomp.wisereport.co.kr/v2/company/c1010001.aspx?cmp_cd=${code}`}
          style={{ width: "700px", height: "100%", minHeight: "600px" }}
        ></iframe>
      </Col>
      <Col flex="250px">
        <PageHeader title="이런 주식은 어떠세요?" />
        {!isLoading ? (
          items.slice(5, 10).map((stock) => (
            <Col key={stock.code}>
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
          ))
        ) : (
          <div style={{ textAlign: "center" }}>
            <LoadingOutlined style={{ marginTop: "30px", fontSize: "50px" }} />
          </div>
        )}
      </Col>
    </Row>
  );
};

export default withRouter(DetailPage);
