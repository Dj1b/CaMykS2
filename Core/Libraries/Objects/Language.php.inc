<?php
/**
 * @brief Language library.
 * <br /> Object to manage language, script and country.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Oct 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Language class.
 */
final class Language {
    /**
     * @var string|null $Language
     */
    private ?string $Language = null;

    /**
     * @var string|null $Script
     */
    private ?string $Script = null;

    /**
     * @var string|null $Country
     */
    private ?string $Country = null;

    /**
     * Class constructor.
     * @param string|array|Language $language
     * @return void
     */
    public function __construct(string|array|Language $language) {
        if ($language instanceof Language) {
            $this->Language = $language->get_Language();
            $this->Script = $language->get_Script();
            $this->Country = $language->get_Country();
            return;
        }

        if (is_array($language) && array_keys($language) === array('Language', 'Script', 'Country')) {
            $this->Language = $language['Language'];
            $this->Script = $language['Script'];
            $this->Country = $language['Country'];
            return;
        }

        if (is_string($language)) {
            $languagesData = language_getLanguagesData();
            if (preg_match('/^([a-z]{2})(_([a-z]{4}))?(_([a-z]{2}))?$/i', $language, $matches)) {
                if (isset($languagesData[strtolower($matches[1])])) {
                    $this->Language = strtolower($matches[1]);
                    $this->Script = (isset($matches[3]) and $matches[3] != '') ? ucwords(strtolower($matches[3])) : null;
                    $this->Country = (isset($matches[5]) and $matches[5] != '') ? strtoupper($matches[5]) : (isset($languagesData[$this->Language]['DefaultCountry']) ? strtoupper($languagesData[$this->Language]['DefaultCountry']) : null);
                }
            }
        }
    }

    /**
     * Return language code.
     * @return string|null
     */
    public function get_Language(): ?string {
        return $this->Language;
    }

    /**
     * Return script code.
     * @return string|null
     */
    public function get_Script(): ?string {
        return $this->Script;
    }

    /**
     * Return country code.
     * @return string|null
     */
    public function get_Country(): ?string {
        return $this->Country;
    }

    /**
     * Return language value as string.
     * @param bool $noDetails
     * @return string
     */
    public function get_value(bool $noDetails=false): string {
        if (is_null($this->Language))
            return '';
            
        if ($noDetails === true)
            return strtolower($this->Language);

        return $this->Language . (is_null($this->Script) ? '' : '_'.$this->Script) . (is_null($this->Country) ? '' : '_'.$this->Country);
    }

    /**
     * Return language value as string (magic method).
     * @return string
     */
    public function __toString(): string {
        return $this->get_value();
    }
}
?>
