package ija.ija2020.homework1.store;

import ija.ija2020.homework1.goods.Goods;
import ija.ija2020.homework1.goods.GoodsItem;
import ija.ija2020.homework1.goods.GoodsShelf;
import ija.ija2020.homework1.helpers.GoodsGet;

import java.util.ArrayList;

/**
 * Prvá úloha do predmetu IJA
 * @file StoreShelf.java
 * @author Timotej Ponek xponek00
 */

public class StoreShelf implements GoodsShelf {

    private ArrayList<Goods> content = new ArrayList<>();

    public StoreShelf() {
    }

    @Override
    public void put(GoodsItem itemToAdd) {
        Goods Adder = itemToAdd.goods();
        if(content.contains(Adder)){
            Goods toBeAdded = content.get(content.indexOf(Adder));
            toBeAdded.addItem(itemToAdd);
        }
        else{
            Goods toBeAdded = new ija.ija2020.homework1.store.StoreGoods(Adder.getName());
            toBeAdded.addItem(itemToAdd);
            content.add(toBeAdded);
        }

    }

    @Override
    public boolean containsGoods(Goods goods) {

        return content.contains(goods);
    }

    @Override
    public GoodsItem removeAny(Goods goods) {
        if(containsGoods(goods)){
            Goods Remover = content.get(content.indexOf(goods));
            return ((GoodsGet) Remover).getItemAndRemove();
        }
        return null;
    }

    @Override
    public int size(Goods goods) {
        return content.contains(goods) ? content.get(content.indexOf(goods)).size() : 0;
    }
}