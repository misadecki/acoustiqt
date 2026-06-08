/**
 * @file axis_painter.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Definicja klasy narzędziowej AxisPainter do rysowania ustandaryzowanych osi wykresów.
 * * Plik zawiera statyczną klasę pomocniczą, która oddziela logikę 
 * rysowania interfejsu (podziałek, wartości, etykiet osi X oraz Y) od logiki 
 * przetwarzania sygnału. Używana głównie przez widżety wizualizacji audio 
 * (np. SpectrumVisualizer i SpectrogramVisualizer) w celu zachowania spójnego 
 * wyglądu marginesów i skalowania jednostek.
 * @version 1.0
 * @date 2026-05-27
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef AXIS_PAINTER_HH
#define AXIS_PAINTER_HH

#include <QPainter>
#include <QString>
#include <QMarginsF>

/**
 * @brief Typ wyliczeniowy określający format jednostek na osiach wykresów.
 */
enum class AxisFormat {
  Auto,    /**< Automatyczne dobieranie jednostki w zależności od zakresu. */
  Hz,      /**< Wymuszenie wyświetlania w hercach [Hz]. */
  kHz,     /**< Wymuszenie wyświetlania w kilohercach [kHz]. */
  Time     /**< Formatowanie wartości jako czas (w sekundach). */
};

/**
 * @brief Klasa narzędziowa do rysowania ustandaryzowanych osi na wykresach.
 */
class AxisPainter {
public:
  /**
   * @brief Rysuje poziomą oś X wraz z podziałką i etykietami.
   * * @param[in] painter -- referencja do obiektu malarza (QPainter) powiązanego z danym widżetem.
   * @param[in] width -- całkowita szerokość widżetu.
   * @param[in] height -- całkowita wysokość widżetu.
   * @param[in] margins -- marginesy określające bezpieczną strefę rysowania wykresu.
   * @param[in] title -- tytuł osi.
   * @param[in] min -- wartość początkowa osi (z lewej strony).
   * @param[in] max -- wartość końcowa osi (z prawej strony).
   * @param[in] numTicks -- liczba głównych znaczników (podziałek) do narysowania na osi.
   * @param[in] format -- format rysowania osi w zależności od wybranej
   * jednostki wybranej z @ref AxisFormat. Domyślnie ustawione na Auto.
   * @param[in] axisColor -- kolor rysowania osi w zależności od motywu.
   * Domyślnie biały.
   */
  static void drawXAxis(QPainter &painter, double width, double height,
                      const QMarginsF &margins, const QString &title, double min, double
                      max, int numTicks, AxisFormat format = AxisFormat::Auto,
                        QColor axisColor = Qt::white);
  /**
   * @brief Rysuje pionową oś Y wraz z podziałką, etykietami i obróconym tytułem.
   * * @param[in] painter -- referencja do obiektu malarza (QPainter).
   * @param[in] height -- całkowita wysokość widżetu.
   * @param[in] margins -- marginesy określające bezpieczną strefę rysowania wykresu.
   * @param[in] title -- tytuł osi.
   * @param[in] min -- wartość dolna osi.
   * @param[in] max -- wartość górna osi.
   * @param[in] numTicks -- liczba głównych znaczników (podziałek) do narysowania na osi.
   * @param[in] format -- format rysowania osi w zależności od wybranej
   * jednostki wybranej z @ref AxisFormat. Domyślnie ustawione na Auto.
   * @param[in] axisColor -- kolor rysowania osi w zależności od motywu.
   * Domyślnie biały.
   */
  static void drawYAxis(QPainter &painter, double height,
                        const QMarginsF &margins, const QString &title, double min, double
                        max, int numTicks, AxisFormat format = AxisFormat::Auto,
                        QColor axisColor = Qt::white);
};

#endif // AXIS_PAINTER_HH
