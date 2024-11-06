package ija.ija2020.homework1.store;

import ija.ija2020.homework1.goods.Goods;
import ija.ija2020.homework1.goods.GoodsItem;
import ija.ija2020.homework1.helpers.GoodsGet;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Objects;

/**
 * Prvá úloha do predmetu IJA
 * @file StoreGoods.java
 * @author Timotej Ponek xponek00
 */

public class StoreGoods implements Goods, GoodsGet {

    private String name;
    private ArrayList<GoodsItem> list = new ArrayList<>();

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        StoreGoods that = (StoreGoods) o;
        return Objects.equals(name, that.name);
    }


    @Override
    public int hashCode() {
        return Objects.hash(name);
    }

    public StoreGoods(String name) {
        this.name = name;
    }

    @Override
    public String getName() {
        return name;
    }

    // Only adds new item to Box
    @Override
    public boolean addItem(GoodsItem goodsItem) {
        return list.contains(goodsItem) ? false : list.add(goodsItem);

    }

    // Creates new item and adds it to box
    @Override
    public GoodsItem newItem(LocalDate localDate) {
        GoodsItem newlyCreated =  new ija.ija2020.homework1.store.StoreGoodsItem(this, localDate);
        list.add(newlyCreated);

        return newlyCreated;
    }

    @Override
    public GoodsItem getItemAndRemove(){
        return list.size() > 0 ? list.remove(0) : null;
    }

    @Override
    public boolean remove(GoodsItem goodsItem) {
        return list.contains(goodsItem) ? list.remove(goodsItem) : false;
    }

    @Override
    public boolean empty() {
        return list.isEmpty();
    }

    @Override
    public int size() {
        return list.size();
    }
}
