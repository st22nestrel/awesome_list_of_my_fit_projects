package ija.ija2020.homework1.helpers;

import ija.ija2020.homework1.goods.GoodsItem;

/**
 * Prvá úloha do predmetu IJA
 * Rozhranie pre pomocnú funkciu ktorú potrebujem na ziskanie a odstránenie GoodsItem zo StoreGoodsShelf
 * @file GoodsGet.java
 * @author Timotej Ponek xponek00
 */

public interface GoodsGet {
    public GoodsItem getItemAndRemove();
}
