interface StockItem {
  name: string;
  code: string;
  diff?: number;
  now_val: number;
  fluctuation: number;
  volume: number;
  wics?: string;
  index?: number;
}

export const makeInfoPageUrl = (code: string) =>
  `https://navercomp.wisereport.co.kr/v2/company/c1010001.aspx?cmp_cd=${code}`;

export const makeIndexItem = (items: StockItem[]): StockItem[] =>
  items.map((c: StockItem, index: number) => ({
    ...c,
    index,
    key: c.code,
  }));

export default StockItem;
