1. Для реализации был выбран алгоритм Sauvola

2. Данный алгоритм в целом работает качественно. Но изображения с малым контрастом вызывают у него проблемы. Это, в частности, вызвано фиксацией в алгоритме максимально возможного стандартного отклонения значения пикселя от среднего. Так, обычно, его берут равным 128 для пикселей 0..255. В качестве улучшения, было внесено вычисление этой величины для каждого изображения в отдельности. Это улучшило результат.

3. Сборка проекта из исходников - build.sh. Для сборки бинаризатора необходима библиотека CImg(https://github.com/dtschump/CImg), изпользуемая для чтения/записи картинок. Вся библиотека расположена в CImg.h фаиле.

4. Для запуска программы необходим фаил, содержащий список путей до изображений. Бинаризованные изображения сохряняются по тем же путям, только с .png на конце.
Пример запуска <code>./binarizer input.txt</code>

5. Результаты работы расположены в папке images вместе с оригинальными изображениями.

6. Скорость работы порядка 0.2 сек на мегапиксель.
