import React, { useState, useCallback, useEffect } from "react";
import { Select, Tooltip, Button, Row, Col } from "antd";
import { SearchOutlined } from "@ant-design/icons";

import StockItemList from "~/components/StockItemList";
import TagService from "~/services/TagService";
import StockItem from "~/utils/interfaces/StockItem";
import StaticTagList from "~/static/TagList";

const TagPage = () => {
  const service = new TagService();

  const [tagList, setTagList] = useState<string[]>([]);
  const [isLoading, setIsLoading] = useState(false);

  const [items, setItems] = useState<StockItem[]>([]);

  useEffect(() => {
    const callingService = async () => {
      setIsLoading(true);
      const result = await service.recommendByDefault();
      const tags = result.default_tags.reduce(
        (prev: string[], curr: string) => {
          if (!prev.find((a) => a === curr)) {
            return [...prev, curr];
          }
          return prev;
        },
        [],
      );

      setTagList(tags);
      setItems(result.content);
      setIsLoading(false);
    };
    callingService();
  }, []);

  const handleChange = useCallback((e) => {
    setTagList(e);
  }, []);

  const handleSearch = useCallback(async () => {
    if (!tagList.length) {
      return;
    }

    const result = await service.recommendByTags(tagList);
    setItems(result.content);
  }, [tagList]);

  return (
    <>
      <Row
        gutter={[8, 0]}
        style={{
          margin: "0",
          width: "100%",
          backgroundColor: "white",
          padding: "20px",
        }}
      >
        <Col flex="auto" style={{ maxWidth: "calc(100% - 50px)" }}>
          <Select
            mode="multiple"
            size="large"
            allowClear
            style={{ width: "100%" }}
            onChange={handleChange}
            value={tagList}
            options={StaticTagList}
            loading={isLoading}
          />
        </Col>
        <Col flex="40px">
          <Tooltip title="검색">
            <Button
              size="large"
              type="primary"
              icon={<SearchOutlined />}
              onClick={handleSearch}
              disabled={!tagList.length}
            />
          </Tooltip>
        </Col>
      </Row>
      <StockItemList stocks={items} />
    </>
  );
};

export default TagPage;
