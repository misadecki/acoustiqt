/**
 * @file mainwindow.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Plik ten zawiera funkcje związane z głównym oknem aplikacji i
 * zasadniczym jej działaniem. 
 * @version 1.0
 * @date 2026-04-22
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QDialog>
#include <QStackedWidget>
#include <QLineEdit> 
#include <QPushButton>
#include <QLabel>
#include <QResizeEvent>
#include "udp_receiver.hh"
#include "spectrum_visualizer.hh"
#include "fft_processor.hh"
#include "spectrogram_visualizer.hh"
#include "influx_client.hh"
#include <QTranslator>

QT_BEGIN_NAMESPACE
/**
 * @namespace Ui
 * @brief Przestrzeń nazw generowana automatycznie przez Qt Designer.
 * @details Oddziela kod interfejsu graficznego wygenerowany z pliku .ui 
 * od logiki aplikacji napisanej w C++.
 */
namespace Ui {
  class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Główne okno aplikacji (GUI).
 * @details Klasa zarządzająca całym interfejsem użytkownika, integrująca 
 * wizualizatory z modułami przetwarzania danych i odbierania pakietów sieciowych.
 * Dziedziczy po QMainWindow, dostarczając standardową strukturę okna.
 */
class MainWindow : public QMainWindow {
  Ui::MainWindow *ui;                             /**< Wskaźnik na wygenerowany interfejs użytkownika. */
  UdpReceiver *receiver;                          /**< Moduł odpowiedzialny za nasłuchiwanie i odbiór strumienia audio. */
  FFTProcessor *processor;                        /**< Silnik przetwarzający sygnał cyfrowy. */
  SpectrumVisualizer *spectrum_visualizer;        /**< Widżet rysujący klasyczne widmo słupkowe (Spectrum). */
  SpectrogramVisualizer *spectrogram_visualizer;  /**< Widżet rysujący spektrogram. */
  bool is_running = true;                         /**< Flaga określająca stan odbierania danych audio. */
  QTimer *timer;                                  /**< Wskaźnik na timer systemowy zarządzający odświeżaniem interfejsu użytkownika. */
  AudioStats latest_stats;                        /**< Struktura przechowująca najświeższe wyniki analizy sygnału. */
  InfluxClient *db_client;                        /**<Wskaźnik na klienta bazy danych InfluxDB. */
  QTranslator app_translator;                     /**< Obiekt silnika tłumaczeń obsługujący podmianę języka aplikacji. */
  const QString org = "KoNaR";                    /**< Nazwa organizacji wykorzystywana m.in. w QSettings */
  const QString application = "AcoustiQt";        /**< Nazwa aplikacji wykorzystywana m.in. w QSettings. */

  /**
  * @brief Alokuje pamięć dla obiektów zawartych w klasie. 
  */
  void initObjects();

  /**
  * @brief Ustawia odpowiednie zależności pomiędzy widokami.
  */
  void setupLayouts();

  /**
  * @brief Realizuje połączenia pomiędzy sygnałami i slotami.
  */
  void initConnections();

  /**
   * @brief Inicjalizuje połączenia sygnałów i slotów dla statystyk.
   * Łączy timer z aktualizacją GUI oraz procesor FFT z odbiorem nowych wyników.
   */
  void initStatsConnections();

  /**
   * @brief Dynamicznie aktualizuje rozmiar i pozycję okienka z wyborem motywu.
   * * @details Funkcja upewnia się, że ramka motywu (theme_frame) zajmuje zawsze 60% 
   * szerokości i wysokości okna głównego, jednocześnie nie pozwalając na jej skurczenie 
   * poniżej określonych limitów (300x200 pikseli). Po zmianie rozmiaru, ramka jest 
   * automatycznie centrowana na środku okna głównego.
   */
  void updateThemeFrameSize();

  /**
   * @brief Inicjalizuje dostępne języki i przygotowuje interfejs do ich obsługi.
   */
  void initLanguages();

  /**
   * @brief Podłącza sygnały i sloty odpowiedzialne za zmianę i obsługę motywów graficznych.
   */
  void connectThemes();

  /**
   * @brief Wczytuje i aplikuje ustawienia aplikacji zapisane podczas poprzedniej sesji.
   */
  void setPreviousSettings();

  /**
   * @brief Odświeża teksty i tłumaczenia w interfejsie po załadowaniu nowego pliku językowego.
   */
  void updateLanguages();

  /**
   * @brief Zeruje statystyki i czyści wykresy, przywracając interfejs do stanu "Brak połączenia".
   */
  void setDisconnectedState();

  /**
   * @brief Dopasowuje rozmieszczenie i geometrię elementów interfejsu.
   */
  void alignObjects();

  /**
   * @brief Inicjalizuje mechanizm Watchdoga monitorujący aktywność i ciągłość odbieranych danych.
   */
  void connectWatchdog();

  /**
   * @brief Podłącza sygnały ze sterownia jednostkami osi (np. z ComboBoxa) do właściwych wizualizatorów.
   */
  void connectAxisUnits();

  /**
   * @brief Ładuje i aplikuje globalny arkusz stylów/motyw do interfejsu aplikacji.
   * @param[in] themePath -- nazwa pliku stylów .qss przechowujący
   * charakterystykę danego motywu.
   */
  void loadTheme(const QString &themeName);
  Q_OBJECT
private slots:
  /**
   * @brief Obsługuje zmianę wartości na suwaku głośności.
   * * @details Funkcja przelicza liniową wartość suwaka (0-100) na wartość wzmocnienia (gain)
   * za pomocą interpolacji wykładniczej, co zapewnia naturalne, liniowe odczucie zmiany
   * głośności dla ucha (lub dla skali decybelowej na wykresach). Wartość 0 powoduje
   * całkowite wyciszenie sygnału (gain = 0.0). Po obliczeniach aktualizuje procesor audio
   * oraz wizualnie synchronizuje pole tekstowe (spinBox).
   * * @param[in] value -- aktualna pozycja suwaka w zakresie od 0 do 100.
   */
  void onVolumeSliderChanged(int value);

  /**
   * @brief Zmienia aktualny język interfejsu aplikacji na podstawie wyboru użytkownika.
   * @param[in] index -- indeks wybranego języka z listy rozwijanej (np. QComboBox).
   */
  void changeLanguage(int index);

  /**
   * @brief Obsługuje kliknięcie przycisku Stop/Resume.
   * Zatrzymuje lub wznawia przepływ danych z odbiornika UDP do procesora FFT
   * oraz odpowiednio aktualizuje tekst na przycisku.
   */
  void on_btn_stop_clicked();

  /**
   * @brief Slot wywoływany cyklicznie przez timer.
   * Pobiera najnowsze statystyki i aktualizuje odpowiednie pola tekstowe (QLineEdit) w GUI.
   * Automatycznie przelicza jednostki (Hz na kHz), jeśli wybrano taką opcję.
   */
  void onTimerTimeout();

  
  /**
   * @brief Aktualizuje statystyki sygnału przechowywane w prywatnym polu klasy.
   *
   * @param[in] stats -- referencja na świeże statystyki sygnału.
   */
  inline void updateStats(const AudioStats &stats) { latest_stats = stats; }
public:
  /**
   * @brief Konstruktor inicjalizujący główne okno aplikacji. Ustawia sygnały i sloty.
   *
   * @param[in] parent -- wskaźnik na rodzica MainWindow.
   */
  explicit MainWindow(QWidget *parent = nullptr);

  /**
   * @brief Domyślny destruktor głównego okna aplkacji.
   */
  ~MainWindow() override;
protected:
  /**
   * @brief Globalny filtr zdarzeń przechwytujący akcje z obserwowanych widżetów.
   * @param[in] watched -- wskaźnik na widżet, do którego skierowane było zdarzenie.
   * @param[in] event -- wskaźnik na obiekt przechwyconego zdarzenia.
   * @return bool -- true, jeśli zdarzenie zostało obsłużone i ma zostać
   * zablokowane dla innych, false w przeciwnym razie.
   */
  bool eventFilter(QObject *watched, QEvent *event) override;
  /**
  * @brief Przelicza rozmiar okna w zależności od skali ustalonej przez
  * użytkownika.
  *
  * @param[in] event -- wskaźnik na zdarzenie.
  */
  void resizeEvent(QResizeEvent *event) override;

  /**
   * @brief Przechwytuje zdarzenie zamykania okna głównego aplikacji.
   * * @details Przed faktycznym zamknięciem programu, funkcja zapisuje aktualny stan 
   * interfejsu (np. pozycję suwaka głośności) do rejestru systemowego lub pliku konfiguracyjnego 
   * za pomocą QSettings. Dzięki temu przy kolejnym uruchomieniu aplikacja przywróci 
   * preferencje użytkownika.
   * * @param[in] event -- wskaźnik na obiekt zdarzenia zamknięcia przekazany przez system Qt.
   */
  void closeEvent(QCloseEvent *event) override;
public slots:
  /**
   * @brief Obsługuje główny widżet wizualizacji widma.
   */
  void mainPageWidget();

  /**
  * @brief Obsługuje widżet spektrogramu.
  */
  void spectrogramPageWidget();
private slots:
  /**
  * @brief Obsługuje wciśnięcie przycisku "Theme".
  */
  void on_btn_theme_clicked();

  /**
  * @brief Obsługuje wciśnięcie przycisku "Cancel".
  */
  void on_btn_cancel_clicked();
};
#endif // MAINWINDOW_HH
